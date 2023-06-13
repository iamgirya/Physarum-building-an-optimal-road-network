import 'package:flutter/material.dart';
import 'package:flutter_hooks/flutter_hooks.dart';
import 'package:hooks_riverpod/hooks_riverpod.dart';
import '../../graph_field/graph_field_state_holders.dart';
import '../setting_manager.dart';
import '../state/simulation_setting_state_holder.dart';
import '../../../support/settings_data.dart';

class SettingsList extends HookConsumerWidget {
  const SettingsList({super.key});

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final graph = ref.watch(nowGraphsFieldGraphStateHolder);
    final state = ref.watch(settingsStateHolder);

    List<String> keyList =
        state.settingsControllers.entries.map((e) => e.key).toList();
    final controllerList = keyList
        .map(
          (e) => useTextEditingController(
            text: state.settingsControllers[e]!.toString(),
          ),
        )
        .toList();

    return graph.isGraphBuilded
        ? const SizedBox()
        : ListView.separated(
            itemBuilder: (context, i) {
              return SettingsCard(
                settingsKey: keyList[i],
                controller: controllerList[i],
              );
            },
            separatorBuilder: (context, i) {
              return const SizedBox(
                height: 10,
              );
            },
            itemCount: controllerList.length,
          );
  }
}

class SettingsCard extends ConsumerWidget {
  const SettingsCard({
    super.key,
    required this.settingsKey,
    required this.controller,
  });

  final String settingsKey;
  final TextEditingController controller;

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    final manager = ref.watch(settingsManager);
    return Column(
      crossAxisAlignment: CrossAxisAlignment.start,
      children: [
        Text(
          settingsMappedData[settingsKey]?.name ?? '!',
        ),
        TextField(
          controller: controller,
          onChanged: (value) =>
              manager.setNewValue(settingsKey, controller.text),
        ),
      ],
    );
  }
}
