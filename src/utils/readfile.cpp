#include <fstream>
#include <vector>

static::std::vector<char>  readFile(const std::string& filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary); //We start reading at end of file to make a buffer for the 
                                                                  //contents corresponding to size

  if (!file.is_open())
    throw std::runtime_error("Failed to open file!");

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize); //This is where we create the aforementioned buffer using the filesize we got from ate | binary
 
  //Now we go back to start of file
  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}
