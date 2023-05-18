import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'package:physarum_flutter/models/graph_model.dart';

final graphsFieldGraphStateHolder = StateProvider<Graph>((ref) {
  return Graph.empty();
});
