#include "encode.h"

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <sstream>

std::string base64_encode(const std::string& s) {
  namespace bai = boost::archive::iterators;

  std::stringstream os;

  // convert binary values to base64 characters
  typedef bai::base64_from_binary
  // retrieve 6 bit integers from a sequence of 8 bit bytes
  <bai::transform_width<const char *, 6, 8> > base64_enc; // compose all the above operations in to a new iterator

  std::copy(base64_enc(s.c_str()), base64_enc(s.c_str() + s.size()),
            std::ostream_iterator<char>(os));

  os << base64_padding[s.size() % 3];
  return os.str();
}

std::string base64_decode(const std::string& s) {
  namespace bai = boost::archive::iterators;

  std::stringstream os;

  typedef bai::transform_width<bai::binary_from_base64<const char *>, 8, 6> base64_dec;

  unsigned int size = s.size();

  // Remove the padding characters, cf. https://svn.boost.org/trac/boost/ticket/5629
  if (size && s[size - 1] == '=') {
    --size;
    if (size && s[size - 1] == '=') --size;
  }
  if (size == 0) return std::string();

  std::copy(base64_dec(s.data()), base64_dec(s.data() + size),
            std::ostream_iterator<char>(os));

  return os.str();
}

std::string base16_encode(const unsigned char s[], const size_t size) {
	std::string output;

	for(size_t i = 0; i < size; i++){
		const unsigned char c = s[i];
		output.append(s_hextable[c], 2);
	}

	return output;
}

std::string base16_encode(const std::string &s) {
	return base16_encode((const unsigned char *)s.c_str(), s.size());
}

//TODO: make faster by using 16 char lookup table and bit shifting?
std::string base16_decode(const std::string &s) {
    assert( (s.size() % 2) == 0 );

    std::string output;

    for(size_t i = 0; i < (s.size() - 1); i += 2){
        //assert both characters in valid set 0-9,a-f
        assert( (((unsigned char)s[i] >= 48) && ((unsigned char)s[i] <= 57)) || (((unsigned char)s[i] >= 97) && ((unsigned char)s[i] <= 102)) );
        assert( (((unsigned char)s[i+1] >= 48) && ((unsigned char)s[i+1] <= 57)) || (((unsigned char)s[i+1] >= 97) && ((unsigned char)s[i+1] <= 102)) );

        for(size_t j = 0; j < 256; j++){
            if( (s_hextable[j][0] == s[i]) && (s_hextable[j][1] == s[i+1])){
                output.append(1, (unsigned char)j);
                break;
            }
        }
    }
    return output;
}
