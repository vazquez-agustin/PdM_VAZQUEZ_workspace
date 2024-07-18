
#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#include <stdbool.h>

typedef bool bool_t;


void debounceFSM_init(void);

void debounceFSM_update(void);

void writeKey(void);

bool_t readKey();

#endif /* API_INC_API_DEBOUNCE_H_ */
