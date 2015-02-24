#include <stdio.h>
#include <stdbool.h>

struct BufferedReader {
    FILE* file;
    char buffer[512 * 1024];
    size_t buffer_size, buffer_offset;
};

void init_BufferedReader(struct BufferedReader* reader, FILE* file) {
    reader->file = file;
    reader->buffer_size = reader->buffer_offset = 0;
}

bool readLine(struct BufferedReader* reader, char * s, int maxL) {
    size_t length = 0;
    while (true) {
        if (reader->buffer_size == reader->buffer_offset) {
            if (feof(reader->file)) {
                break;
            }
            reader->buffer_offset = 0;
            reader->buffer_size = fread(reader->buffer, 1, sizeof(reader->buffer), reader->file);
            continue;
        }
        if (reader->buffer[reader->buffer_offset] == '\n') {
            reader->buffer_offset++;
            s[length] = '\0';
            return true;
        } else {
            s[length++] = reader->buffer[reader->buffer_offset++];
            assert(length <= maxL);
        }
    }
    s[length] = '\0';
    return length > 0;
}
