#include <cstdint>

namespace mavlink {
  /* the sequence number of the system.
     do NOT use _sequenceNumber directly to craft a message.
     use sequenceNumber() instead, which guarantee you that the variable is incremented at each call
   */
  volatile uint8_t _sequenceNumber = 0;
}
