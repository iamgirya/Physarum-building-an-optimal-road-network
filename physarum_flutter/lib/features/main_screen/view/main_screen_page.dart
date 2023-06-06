import 'package:flutter/material.dart';

import '../../graph_field/ui/graph_field_view.dart';
import 'graph_edit_drawer.dart';
import 'main_interface_column.dart';

import '../../setting_panel/view/setting_panel_view.dart';

class MainPage extends StatelessWidget {
  const MainPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: const Text('Physarum Network'),
      ),
      body: const Stack(
        children: [
          Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            children: [
              GraphDrawField(
                sizeOfField: 200,
                sizeOfPixel: 3,
                isBest: false,
              ),
              SizedBox(
                width: 15,
              ),
              MainInterfaceColumn(),
              SizedBox(
                width: 15,
              ),
              SizedBox(
                width: 600,
                height: 600,
                child: SettingsPanel(),
              )
            ],
          ),
          GraphEditDrawer(),
        ],
      ),
    );
  }
}
