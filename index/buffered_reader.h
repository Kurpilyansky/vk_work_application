#include <stdio.h>
#include <stdbool.h>

struct BufferedReader {
    FILE* file;
    char buffer[64 * 1024];
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
            if (fgets(reader->buffer, sizeof(reader->buffer), reader->file) == NULL) {
                break;
            }
            reader->buffer_offset = 0;
            reader->buffer_size = strlen(reader->buffer);
            continue;
        }
        if (reader->buffer[reader->buffer_offset] == '\n') {
            reader->buffer_offset++;
            s[length] = '\0';
            return true;
        } else {
            s[length++] = reader->buffer[reader->buffer_offset++];
            assert(length < maxL);
        }
    }
    s[length] = '\0';
    return length > 0;
}
