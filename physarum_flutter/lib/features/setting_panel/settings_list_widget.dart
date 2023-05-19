import 'package:flutter/material.dart';
import 'package:flutter_riverpod/flutter_riverpod.dart';
import 'package:physarum_flutter/features/graph_field/graph_field_state_holders.dart';
import 'package:physarum_flutter/features/main_screen/view/main_screen_button.dart';
import 'package:physarum_flutter/features/setting_panel/setting_manager.dart';
import 'package:physarum_flutter/features/setting_panel/simulation_setting_state_holder.dart';
import 'package:physarum_flutter/support/settings_data.dart';

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
        ? const _IsAlgoWorkingMessage()
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

class _IsAlgoWorkingMessage extends StatelessWidget {
  const _IsAlgoWorkingMessage();

  @override
  Widget build(BuildContext context) {
    return const Center(
      child: Text(
        'Для изменения параметров нужно сбросить нынешнюю симуляцию',
      ),
    );
  }
}
