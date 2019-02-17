#include <CircularBuffer.h>

#define WINDOW_SIZE 7
#define HISTORY_SIZE 10
#define ZERO_THRESHOLD 5
#define BUFFER_RESET_THRESHOLD 85

CircularBuffer<int, HISTORY_SIZE> historyBuffer;
CircularBuffer<int, WINDOW_SIZE> windowBuffer;

int prevInput = 0;

int filter(int input) {
  historyBuffer.push(input);

  if (input != 0) {
    return input;
  }

  int zerosCount = 0;
  using index_t = decltype(historyBuffer)::index_t;

  for (index_t i = historyBuffer.size(); i > 0; i--) {
    if (historyBuffer[i] == 0) {
      if (++zerosCount >= ZERO_THRESHOLD) {
        return 0;
      }
    } else {
      return historyBuffer[i];
    }
  }
}

int getEqualized(int input) {
//  Serial.print("Original input: ");
//  Serial.println(input);

  int lastBufferedValue = windowBuffer[windowBuffer.size() - 1];

  if (abs(lastBufferedValue - input) >= BUFFER_RESET_THRESHOLD) {
      windowBuffer.clear();
  }

  windowBuffer.push(input);
  float avg = 0.0;

  using index_t = decltype(windowBuffer)::index_t;
  for (index_t i = 0; i < windowBuffer.size(); i++) {
    avg += windowBuffer[i] / windowBuffer.size();
  }

  prevInput = input;

  return (int) round(avg);
}
