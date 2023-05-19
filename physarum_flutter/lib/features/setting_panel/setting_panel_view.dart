import 'package:flutter/material.dart';
import 'package:physarum_flutter/features/setting_panel/vertex_list_view.dart';

class SettingsPanel extends StatelessWidget {
  const SettingsPanel({super.key});

  @override
  Widget build(BuildContext context) {
    return DefaultTabController(
      length: 2,
      child: Scaffold(
        appBar: AppBar(
          title: const TabBar(
            tabs: [
              Tab(
                text: 'Вершины',
              ),
              Tab(
                text: 'Расширенные настройки',
              ),
            ],
          ),
        ),
        body: const TabBarView(
          children: [
            VertexList(),
            Center(
              child: Text('1'),
            ),
          ],
        ),
      ),
    );
  }
}
