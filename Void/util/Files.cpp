#include "Files.hpp"

namespace Void 
{
    /**
     * Recursively iterate over a directory.
     */
    void Files::walk(STRING directory, LIST_T<PATH>& result)
    {
        // create new directory iterator

        for (DIR_ENTRY entry : DIR_ITERATOR(directory))
        {
            // get the path of the entry

            PATH path = entry.path();
            
            // continue searching if entry is a directory

            if (IS_DIRECTORY(path))
            {
                walk(path.generic_string(), result);
            }

            // entry is a file, append file path to result

            else
            {
                result.push_back(path);
            }
        }
    }

    /**
     * Read all lines from the input stream.
     */
    LIST Files::readLines(FILE_INPUT_STREAM& stream)
    {
        LIST lines;
        STRING line;
        
        // read file line by line

        while (GET_LINE(stream, line))
        {
            lines.push_back(line);
        }

        return lines;
    }

    /**
     * Read the content of a file.
     */
    STRING Files::readAll(FILE_INPUT_STREAM& stream)
    {
        STRING line, content;

        // read file line by line

        while (GET_LINE(stream, line))
        {
            content += line + '\n';
        }

        return content;
    }
}
