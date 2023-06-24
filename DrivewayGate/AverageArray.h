class AverageArray {
  private:
    uint8_t mIndex;
    uint16_t mAverageRange;
    uint16_t* pArray;

    float ProduceAverage() {
      float avg = 0;
      for(int i = 0; i < this->mAverageRange; i++) {
        avg += this->pArray[i];
      }

      avg /= this->mAverageRange;
      return avg;
    }

  public:
    AverageArray(uint16_t range) {
      this->mAverageRange = range;
      this->pArray = new uint16_t[this->mAverageRange];

      InitializeArray();
    }

    ~AverageArray() {
      delete[] pArray;
    }

    float Insert(uint16_t value) {
      this->mIndex = (this->mIndex + 1) % this->mAverageRange;
      this->pArray[this->mIndex] = value;

      return ProduceAverage();
    }

    void InitializeArray() {
      for(int i = 0; i < this->mAverageRange; i++) {
        this->pArray[i] = 0;
      }
    }
};