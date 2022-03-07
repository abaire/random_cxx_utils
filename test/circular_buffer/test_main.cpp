#define BOOST_TEST_MODULE CircularBufferTests
#include <boost/test/unit_test.hpp>
#include <string>

#include "circular_buffer.h"
#include "circular_buffer_impl.h"

BOOST_AUTO_TEST_SUITE(circular_buffer_suite)

BOOST_AUTO_TEST_CASE(create_destroy_test) {
  CircularBuffer cb = CBCreate(256);
  BOOST_TEST(cb != nullptr);

  BOOST_TEST(CBCapacity(cb) == 256);
  BOOST_TEST(CBAvailable(cb) == 0);
  BOOST_TEST(CBFreeSpace(cb) == 256);

  CBDestroy(cb);
}

BOOST_AUTO_TEST_CASE(size_test) {
  CircularBuffer cb = CBCreate(256);
  BOOST_TEST(cb != nullptr);

  BOOST_TEST(CBCapacity(cb) == 256);
  BOOST_TEST(CBAvailable(cb) == 0);
  BOOST_TEST(CBFreeSpace(cb) == 256);

  auto cbi = static_cast<CircularBufferImpl *>(cb);
  cbi->read = 0;
  cbi->write = 256;
  BOOST_TEST(CBAvailable(cb) == 256);
  BOOST_TEST(CBFreeSpace(cb) == 0);

  cbi->read = 0;
  cbi->write = 256 - 16;
  BOOST_TEST(CBAvailable(cb) == 240);
  BOOST_TEST(CBFreeSpace(cb) == 16);

  cbi->read = 256;
  cbi->write = 0;
  BOOST_TEST(CBAvailable(cb) == 1);
  BOOST_TEST(CBFreeSpace(cb) == 255);

  cbi->read = 1;
  cbi->write = 0;
  BOOST_TEST(CBAvailable(cb) == 256);
  BOOST_TEST(CBFreeSpace(cb) == 0);

  cbi->read = 17;
  cbi->write = 16;
  BOOST_TEST(CBAvailable(cb) == 256);
  BOOST_TEST(CBFreeSpace(cb) == 0);

  cbi->read = 17;
  cbi->write = 0;
  BOOST_TEST(CBAvailable(cb) == 240);
  BOOST_TEST(CBFreeSpace(cb) == 16);

  CBDestroy(cb);
}

BOOST_AUTO_TEST_CASE(read_write_test) {
  uint8_t test_data[16];
  for (auto i = 0; i < sizeof(test_data); ++i) {
    test_data[i] = i;
  }

  CircularBuffer cb = CBCreate(16);
  BOOST_TEST(cb != nullptr);
  BOOST_TEST(CBCapacity(cb) == 16);

  uint8_t *src = test_data;
  BOOST_TEST(CBWrite(cb, src, 15));
  BOOST_TEST(CBAvailable(cb) == 15);
  BOOST_TEST(CBFreeSpace(cb) == 1);
  BOOST_TEST(!CBWrite(cb, src, 15));
  BOOST_TEST(CBAvailable(cb) == 15);
  BOOST_TEST(CBFreeSpace(cb) == 1);
  src += 15;
  BOOST_TEST(CBWrite(cb, src, 1));
  BOOST_TEST(CBAvailable(cb) == 16);
  BOOST_TEST(CBFreeSpace(cb) == 0);
  BOOST_TEST(!CBWrite(cb, src, 1));

  uint8_t read_buffer[16];
  BOOST_TEST(CBRead(cb, read_buffer, 16));
  BOOST_TEST(CBAvailable(cb) == 0);
  BOOST_TEST(CBFreeSpace(cb) == 16);
  BOOST_TEST(!memcmp(read_buffer, test_data, sizeof(test_data)));

  CBDestroy(cb);
}

BOOST_AUTO_TEST_SUITE_END()
