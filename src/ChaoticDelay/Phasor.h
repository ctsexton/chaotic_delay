class Phasor {
   public:
    Phasor() {}

    double advance(double increment) {
      auto new_phase = phase + increment;
      while (new_phase >= max) {
          new_phase = new_phase - max;
      }
      while (new_phase < 0) {
          new_phase = new_phase + max;
      }
      phase = new_phase;
      return phase;
    };
    void setMax(double value) { max = value; };
    void reset() { phase = 0; }

   private:
    double phase = 0;
    double max = 0;
};
