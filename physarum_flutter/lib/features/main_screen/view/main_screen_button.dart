import 'package:flutter/material.dart';

class MainScreenButton extends StatelessWidget {
  final Function(BuildContext context) onPressed;
  final String title;

  const MainScreenButton({
    super.key,
    required this.onPressed,
    required this.title,
  });

  @override
  Widget build(BuildContext context) {
    return TextButton(
      onPressed: () => onPressed(context),
      child: Text(title),
    );
  }
}
