//***************************************************************************
//* Copyright (c) 2021 Saint Petersburg State University
//* All Rights Reserved
//* See file LICENSE for details.
//***************************************************************************

#include "binning_propagation.hpp"
#include "projects/bin_refine/binning.hpp"

using namespace bin_stats;
using namespace debruijn_graph;

SoftBinsAssignment BinningPropagation::PropagateBinning(BinStats& bin_stats) {
  unsigned iteration_step = 0;
  SoftBinsAssignment state = InitLabels(bin_stats), new_state(state);
  while (true) {
      FinalIteration is_final_iteration = PropagationIteration(state, new_state,
                                                               bin_stats, iteration_step++);
      if (is_final_iteration) {
          StateToBinning(new_state, bin_stats);
          return new_state;
      }

      std::swap(state, new_state);
  }
}

void BinningPropagation::StateToBinning(const SoftBinsAssignment& cur_state, BinStats& bin_stats) {
    std::vector<EdgeId> binned;
    for (EdgeId e : bin_stats.unbinned_edges()) {
        auto assignment = ChooseMostProbableBins(cur_state.at(e).labels_probabilities);
        if (assignment.empty())
            continue;

        binned.push_back(e);
        bin_stats.edges_binning()[e] = std::move(assignment);
    }

    for (EdgeId e : binned)
        bin_stats.unbinned_edges().erase(e);
}

BinningPropagation::FinalIteration BinningPropagation::PropagationIteration(SoftBinsAssignment& new_state,
                                              const SoftBinsAssignment& cur_state,
                                              const BinStats& bin_stats, unsigned iteration_step) {
  double sum_diff = 0.0, after_prob = 0;

  for (EdgeId e : bin_stats.unbinned_edges()) {
    const EdgeLabels& edge_labels = cur_state.at(e);
    auto& next_probs = new_state.at(e).labels_probabilities;

    std::fill(next_probs.begin(), next_probs.end(), 0.0);
    double e_sum = 0.0;
    // Not used now, but might be in the future
    double incoming_weight = 1.0; // / double(g_.IncomingEdgeCount(g_.EdgeStart(e)));
    for (EdgeId neighbour : g_.IncomingEdges(g_.EdgeStart(e))) {
        if (neighbour == e)
            continue;

        PropagateFromEdge(next_probs, neighbour, cur_state, incoming_weight, e_sum);
    }
    for (EdgeId neighbour : g_.OutgoingEdges(g_.EdgeEnd(e))) {
        if (neighbour == e)
            continue;

        PropagateFromEdge(next_probs, neighbour, cur_state, incoming_weight, e_sum);
    }

    // Note that e_sum is actually equals to # of non-empty predecessors,
    // however, we use true sum here in order to compensate for possible
    // rounding-off errors
    if (e_sum == 0.0)
        continue;

    double inv_sum = 1.0 / e_sum;
    for (size_t i = 0; i < next_probs.size(); ++i) {
        next_probs[i] *= inv_sum;
        after_prob += next_probs[i];
        sum_diff += std::abs(next_probs[i] - edge_labels.labels_probabilities[i]);
    }
  }

  VERBOSE_POWER_T2(iteration_step, 0,
                   "Iteration " << iteration_step << ", prob " << after_prob << ", diff " << sum_diff << ", eps " << sum_diff / after_prob);

  // FIXME: We need to refine the condition:
  // We always need to ensure that all edges are reached (so, after_prob will be stable)
  return (sum_diff / after_prob <= eps_);
}

void BinningPropagation::PropagateFromEdge(std::vector<double>& labels_probabilities,
                                           debruijn_graph::EdgeId neighbour,
                                           const SoftBinsAssignment& cur_state,
                                           double weight,
                                           double& sum) {
    const auto& neig_probs = cur_state.at(neighbour).labels_probabilities;
    for (size_t i = 0; i < labels_probabilities.size(); ++i) {
        double p = neig_probs[i] * weight;
        labels_probabilities[i] += p;
        sum += p;
    }
}

SoftBinsAssignment BinningPropagation::InitLabels(const BinStats& bin_stats) {
    SoftBinsAssignment state;
    for (EdgeId e : bin_stats.graph().edges())
        state.emplace(e, EdgeLabels(e, bin_stats));

    EqualizeConjugates(state, bin_stats);

    return state;
}

void BinningPropagation::EqualizeConjugates(SoftBinsAssignment& state, const BinStats& bin_stats) {
    for (EdgeId e : bin_stats.unbinned_edges()) {
        EdgeLabels& edge_labels = state.at(e);
        EdgeLabels& conjugate_labels = state.at(g_.conjugate(e));
        for (size_t i = 0; i < edge_labels.labels_probabilities.size(); ++i) {
            edge_labels.labels_probabilities[i] = (edge_labels.labels_probabilities[i] + conjugate_labels.labels_probabilities[i]) / 2;
            conjugate_labels.labels_probabilities[i] = edge_labels.labels_probabilities[i];
        }
    }
}

std::unordered_set<bin_stats::BinStats::BinId> BinningPropagation::ChooseMostProbableBins(const std::vector<double>& labels_probabilities) {
  double max_probability = 0.0;
  for (double p : labels_probabilities)
    max_probability = std::max(max_probability, p);

  if (max_probability == 0.0)
    return {};

  std::unordered_set<bin_stats::BinStats::BinId> most_probable_bins;
  for (size_t i = 0; i < labels_probabilities.size(); ++i) {
    if (labels_probabilities[i] == max_probability)
      most_probable_bins.insert(i);
  }

  return most_probable_bins;
}
