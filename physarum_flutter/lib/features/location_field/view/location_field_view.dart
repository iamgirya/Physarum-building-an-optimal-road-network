import '../../../support/draw_field.dart';
import '../models/location.dart';
import 'location_painter.dart';

class LocationDrawField extends DrawField {
  LocationDrawField({
    super.key,
    required super.sizeOfField,
    required super.title,
    required Location location,
  }) : super(
          builder: (widthScale, heightScale, ref) => null,
          painterConstructor: (widthScale, heightScale) => LocationPainter(
            location,
            widthScale,
            heightScale,
          ),
        );
}
