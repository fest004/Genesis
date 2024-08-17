#include <fstream>
#include <vector>

static::std::vector<char>  read_file(const std::string& filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary); //We start reading at end of file to make a buffer for the 
                                                                  //contents corresponding to size

  if (!file.is_open())
    throw std::runtime_error("Failed to open file!");

  size_t file_size = (size_t)file.tellg();
  std::vector<char> buffer(file_size); //This is where we create the aforementioned buffer using the filesize we got from ate | binary
 
  //Now we go back to start of file
  file.seekg(0);
  file.read(buffer.data(), file_size);

  file.close();

  return buffer;
}
