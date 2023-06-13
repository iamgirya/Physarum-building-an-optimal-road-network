import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../../graph_field/ui/graph_field_view.dart';

import '../state/main_screen_state_holder.dart';

class GraphEditDrawer extends ConsumerStatefulWidget {
  final double sizeOfFied;
  const GraphEditDrawer({super.key, required this.sizeOfFied});

  @override
  ConsumerState<GraphEditDrawer> createState() => _GraphEditDrawerState();
}

class _GraphEditDrawerState extends ConsumerState<GraphEditDrawer> {
  @override
  Widget build(BuildContext context) {
    final state = ref.watch(mainScreenStateHolder);
    return AnimatedPositioned(
      right: state.isNeedRestart ? -widget.sizeOfFied : 0,
      duration: const Duration(milliseconds: 600),
      curve: Curves.fastOutSlowIn,
      child: Container(
        alignment: Alignment.topRight,
        height: MediaQuery.of(context).size.height,
        width: widget.sizeOfFied,
        child: GraphDrawField(
          sizeOfField: widget.sizeOfFied,
          isBest: true,
        ),
      ),
    );
  }
}
