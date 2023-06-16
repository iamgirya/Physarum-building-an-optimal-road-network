import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../../graph_field/graph_field_state_holders.dart';

import '../../graph_field/view/graph_draw_field.dart';
import '../../location_field/location_state_holders.dart';
import '../../location_field/view/location_field_view.dart';
import '../state/main_screen_state_holder.dart';

class DrawFieldDrawer extends ConsumerWidget {
  final double sizeOfFied;
  const DrawFieldDrawer({super.key, required this.sizeOfFied});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final state = ref.watch(mainScreenStateHolder);
    return AnimatedPositioned(
      right: state.isNeedRestart ? -sizeOfFied : 0,
      duration: const Duration(milliseconds: 600),
      curve: Curves.fastOutSlowIn,
      child: Container(
        alignment: Alignment.topRight,
        height: MediaQuery.of(context).size.height,
        width: sizeOfFied,
        child: state.isBestOnDrawer
            ? Consumer(
                builder: (context, ref, child) {
                  final graph = ref.watch(bestGraphsFieldGraphStateHolder);
                  return GraphDrawField(
                    sizeOfField: sizeOfFied,
                    title: 'Лучший граф',
                    graph: graph,
                    isEditor: false,
                  );
                },
              )
            : Consumer(
                builder: (context, ref, child) {
                  final location = ref.watch(locationStateHolder);
                  return LocationDrawField(
                    sizeOfField: sizeOfFied,
                    title: 'Карта агентов',
                    location: location,
                  );
                },
              ),
      ),
    );
  }
}
