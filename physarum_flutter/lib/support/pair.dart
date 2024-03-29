class Pair<T extends num> implements Comparable<Pair<T>> {
  final T first;
  final T second;
  Pair(this.first, this.second);

  @override
  bool operator ==(Object other) =>
      (other as Pair).first == first && other.second == second;

  @override
  int get hashCode => (first.toString() + second.toString()).hashCode;

  @override
  int compareTo(Pair other) {
    return first > other.first || first == other.first && second > other.second
        ? 1
        : first == other.first && second == other.second
            ? 0
            : -1;
  }
}
