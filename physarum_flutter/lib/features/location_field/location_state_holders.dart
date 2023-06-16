import 'package:flutter_riverpod/flutter_riverpod.dart';

import 'models/location.dart';

final locationStateHolder =
    NotifierProvider<LocationNotifier, Location>(() => LocationNotifier());

class LocationNotifier extends Notifier<Location> {
  @override
  Location build() => Location.empty();

// ignore: avoid_public_notifier_properties
  @override
  Location get state => super.state;

  void update(Location Function(Location) func) {
    super.state = func(super.state);
  }
}
