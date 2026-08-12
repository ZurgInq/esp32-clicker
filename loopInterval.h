struct LoopInterval {
  LoopInterval(unsigned long delay);

  // Если после последнего вызова loopInterval() прошло больше delay времени, то возвращает разница. В ином случае 0.
  unsigned long loopInterval(unsigned long now);

  private:
    unsigned long last = 0;
    unsigned long delay = 0;
};
