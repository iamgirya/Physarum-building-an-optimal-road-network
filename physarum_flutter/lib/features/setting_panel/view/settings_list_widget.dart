import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import '../../graph_field/graph_field_state_holders.dart';
import '../../main_screen/view/main_screen_button.dart';
import '../setting_manager.dart';
import '../simulation_setting_state_holder.dart';
import '../../../support/settings_data.dart';

class SettingsList extends ConsumerStatefulWidget {
  const SettingsList({super.key});

  @override
  ConsumerState<SettingsList> createState() => _SettingsListState();
}

class _SettingsListState extends ConsumerState<SettingsList> {
  @override
  Widget build(BuildContext context) {
    final manager = ref.watch(settingsManager);
    final graph = ref.watch(nowGraphsFieldGraphStateHolder);
    final state = ref.watch(settingsStateHolder);
    final list = state.settingsControllers.entries.toList();
    return graph.isGraphBuilded
        ? const SizedBox()
        : Column(
            children: [
              Expanded(
                child: ListView.separated(
                  itemBuilder: (context, i) {
                    return Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Text(
                          settingsName[list[i].key] ?? '!',
                        ),
                        TextField(
                          controller: list[i].value,
                        ),
                      ],
                    );
                  },
                  separatorBuilder: (context, i) {
                    return const SizedBox(
                      height: 10,
                    );
                  },
                  itemCount: list.length,
                ),
              ),
              MainScreenButton(
                onPressed: (_) => manager.onSaveSetting(),
                title: 'Сохранить',
              ),
            ],
          );
  }
}
