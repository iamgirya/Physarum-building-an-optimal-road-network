import 'package:flutter/material.dart';
import 'package:physarum_flutter/models/graph_model.dart';
import 'package:physarum_flutter/ui/my_app.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';

void main() {
  final a = Graph(towns: [], exitPoints: [], graph: []);
  a.towns.add(1);
  runApp(const ProviderScope(child: MyApp()));
}
