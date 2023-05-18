import 'package:flutter_riverpod/flutter_riverpod.dart';

import '../../models/graph_model.dart';

final graphsFieldGraphStateHolder = StateProvider<Graph>((ref) {
  return const Graph(exitPoints: [], towns: [], graph: []);
});
