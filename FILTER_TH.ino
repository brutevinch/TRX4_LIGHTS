#include <CircularBuffer.h>

#define TH_WINDOW_SIZE 15
#define TH_HISTORY_SIZE 15
#define TH_ZERO_THRESHOLD 13
#define TH_BUFFER_RESET_THRESHOLD 85

CircularBuffer<int, TH_HISTORY_SIZE> th_historyBuffer;
CircularBuffer<int, TH_WINDOW_SIZE> th_windowBuffer;

int th_prevInput = 0;

int th_filter(int input) {
  th_historyBuffer.push(input);

  if (input != 0) {
    return input;
  }

  int zerosCount = 0;
  using index_t = decltype(th_historyBuffer)::index_t;

  for (index_t i = th_historyBuffer.size(); i > 0; i--) {
    if (th_historyBuffer[i] == 0) {
      if (++zerosCount >= TH_ZERO_THRESHOLD) {
        return 0;
      }
    } else {
      return th_historyBuffer[i];
    }
  }
}

int getThEqualized(int input) {
//  Serial.print("Original input: ");
//  Serial.println(input);

  int lastBufferedValue = th_windowBuffer[th_windowBuffer.size() - 1];

  if (abs(lastBufferedValue - input) >= TH_BUFFER_RESET_THRESHOLD) {
      th_windowBuffer.clear();
  }

  th_windowBuffer.push(input);
  float avg = 0.0;

  using index_t = decltype(th_windowBuffer)::index_t;
  for (index_t i = 0; i < th_windowBuffer.size(); i++) {
    avg += th_windowBuffer[i] / th_windowBuffer.size();
  }

  th_prevInput = input;

  return (int) round(avg);
}
