import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_cpp_ffi/physarum_core.dart' as ffi;
import 'package:physarum_cpp_ffi/physarum_cpp_execute_func.dart';
import 'package:physarum_cpp_ffi/physarum_flutter_adapter_model.dart';
import 'package:physarum_flutter/features/graph_field/graph_field_manager.dart';
import 'package:physarum_flutter/models/graph_model.dart';
import 'package:physarum_flutter/models/pair.dart';

final physarumManager = Provider<PhysarumManager>((ref) {
  return PhysarumManager(
    graphFieldManager: ref.watch(graphFieldManager),
  );
});

class PhysarumManager {
  static const int iterationPerStep = 100;
  final GraphFieldManager graphFieldManager;
  PhysarumManager({
    required this.graphFieldManager,
  });

  Future<void> onExecuteButtonTap() async {
    final network = await ffi.bindings.executeAsync(iterationPerStep, 1);
    graphFieldManager.setNewGraph(_parseNetworkToGraph(network));
    int count = 50;
    _callNextStep(count);
  }

  Future<void> _callNextStep(int stepCount) async {
    if (stepCount == 0) {
      return;
    } else {
      final network = await ffi.bindings.executeAsync(iterationPerStep, 0);
      graphFieldManager.setNewGraph(_parseNetworkToGraph(network));
      _callNextStep(stepCount - 1);
    }
  }

  Graph _parseNetworkToGraph(SlimeMoldNetwork network) {
    final graph = Graph.empty();
    for (int i = 0; i < network.exitPoints.length; i++) {
      graph.towns.add(network.towns[i]);
      graph.exitPoints
          .add(Pair(network.exitPoints[i][0], network.exitPoints[i][1]));
      graph.graph.add(network.graph[i]);
    }
    return graph;
  }
}
