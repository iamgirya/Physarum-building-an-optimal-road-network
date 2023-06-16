import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_cpp_ffi/physarum_bindings.dart' as bindings;

import '../../support/pair.dart';
import '../graph_field/models/graph_model.dart';
import '../location_field/models/location.dart';

final physarumRepositoryProvider = Provider<PhysarumRepository>((ref) {
  return PhysarumRepository();
});

class PhysarumRepository {
  void setUpTowns(List<Pair<int>> points, List<int> towns) {
    bindings.setUpTowns(points.map((e) => [e.first, e.second]).toList(), towns);
  }

  void setUpSimulation(Map<String, num> settings) {
    bindings.setUpSimulation(settings);
  }

  Location getLocation() {
    bindings.Location result = bindings.getLocation();
    return Location(
      trailMap: List.from(result.trailMap),
      exitPoints:
          result.exitPoints.map((e) => Pair<double>(e[0], e[1])).toList(),
    );
  }

  Graph getGraph(bool isNeedBest) {
    bindings.Graph result = bindings.getGraph(isNeedBest);
    return Graph(
      towns: List.from(result.towns),
      exitPoints: result.exitPoints.map((e) => Pair(e[0], e[1])).toList(),
      graph: List.from(result.graph),
    );
  }

  List<double> getBestMetrics() {
    return bindings.getBestMetrics();
  }

  Future<void> makeStep(int stepCount) async {
    return await bindings.makeStep(stepCount);
  }
}
