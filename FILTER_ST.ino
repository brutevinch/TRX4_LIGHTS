#include <CircularBuffer.h>

#define ST_WINDOW_SIZE 7
#define ST_HISTORY_SIZE 10
#define ST_ZERO_THRESHOLD 5
#define ST_BUFFER_RESET_THRESHOLD 85

CircularBuffer<int, ST_HISTORY_SIZE> st_historyBuffer;
CircularBuffer<int, ST_WINDOW_SIZE> st_windowBuffer;

int st_prevInput = 0;

int st_filter(int input) {
  st_historyBuffer.push(input);

  if (input != 0) {
    return input;
  }

  int zerosCount = 0;
  using index_t = decltype(st_historyBuffer)::index_t;

  for (index_t i = st_historyBuffer.size(); i > 0; i--) {
    if (st_historyBuffer[i] == 0) {
      if (++zerosCount >= ST_ZERO_THRESHOLD) {
        return 0;
      }
    } else {
      return st_historyBuffer[i];
    }
  }
}

int getStEqualized(int input) {
//  Serial.print("Original input: ");
//  Serial.println(input);

  int lastBufferedValue = st_windowBuffer[st_windowBuffer.size() - 1];

  if (abs(lastBufferedValue - input) >= ST_BUFFER_RESET_THRESHOLD) {
      st_windowBuffer.clear();
  }

  st_windowBuffer.push(input);
  float avg = 0.0;

  using index_t = decltype(st_windowBuffer)::index_t;
  for (index_t i = 0; i < st_windowBuffer.size(); i++) {
    avg += st_windowBuffer[i] / st_windowBuffer.size();
  }

  st_prevInput = input;

  return (int) round(avg);
}
