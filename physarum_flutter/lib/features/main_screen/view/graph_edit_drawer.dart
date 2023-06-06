import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../../graph_field/ui/graph_field_view.dart';

import '../main_screen_state_holder.dart';

class GraphEditDrawer extends ConsumerStatefulWidget {
  const GraphEditDrawer({super.key});

  @override
  ConsumerState<GraphEditDrawer> createState() => _GraphEditDrawerState();
}

class _GraphEditDrawerState extends ConsumerState<GraphEditDrawer> {
  @override
  Widget build(BuildContext context) {
    const double width = 600;
    final state = ref.watch(mainScreenStateHolder);
    return AnimatedPositioned(
      right: state.isAlgoWorking ? 0 : -width,
      duration: const Duration(seconds: 1),
      curve: Curves.fastOutSlowIn,
      child: Container(
        alignment: Alignment.topRight,
        height: MediaQuery.of(context).size.height,
        width: width,
        child: const GraphDrawField(
          sizeOfField: 200,
          sizeOfPixel: 3,
          isBest: true,
        ),
      ),
    );
  }
}
