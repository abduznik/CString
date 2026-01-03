#include <cstring.h>
#include <stdio.h>
#include <string.h>

struct String {
	size_t length;
	char *str;
};

String *string_from(Arena *arena, char *str) {
	int strlen;
	String *st;
	char *new_str;

	strlen = 0;
	st = (String *)arena_alloc(arena, sizeof(String));
	while (str[strlen] != '\0') {
		strlen++;
	}
	new_str = (char *)arena_alloc(arena, strlen + 1); // +1 for null terminator
	memcpy(new_str, str, strlen);
	st->str = new_str;
	st->length = strlen;

	return st;
}

String *string_clone(Arena *arena, String *str) {
    String *st;
    char *new_str;

    st = (String *)arena_alloc(arena, sizeof(String));
    
    new_str = (char *)arena_alloc(arena, str->length + 1);

    memcpy(new_str, str->str, str->length);
    new_str[str->length] = '\0';

    st->str = new_str;
    st->length = str->length;

    return st;
}

String *string_concat(Arena *arena, String *str1, String *str2) {
	String *st;
	char *new_str;

	st = (String *)arena_alloc(arena, sizeof(String));
	new_str = (char *)arena_alloc(arena, str1->length + str2->length +
											 1); // +1 for null terminator

	memcpy(new_str, str1->str, str1->length);
	memcpy(new_str + str1->length, str2->str, str2->length);
	new_str[str1->length + str2->length] = '\0';

	st->str = new_str;
	st->length = str1->length + str2->length;
	return st;
}

int string_len(String *str) { return str->length; }

String *string_sub(Arena *arena, String *str, int begin, int end) {
	size_t substring_length;
	String *st;
	char *new_str;

	substring_length = 0;
	st = (String *)arena_alloc(arena, sizeof(String));
	if (begin < end && begin >= 0 && begin < str->length &&
		end <= str->length) {
		substring_length = (end - begin);
		new_str = (char *)arena_alloc(arena, substring_length + 1);
		memcpy(new_str, str->str + begin, substring_length);

		new_str[substring_length] = '\0';
		st->str = new_str;
		st->length = substring_length;
	} else {
		new_str = (char *)arena_alloc(arena, 1);
		new_str[0] = '\0';
		st->str = new_str;
		st->length = 0;
	}

	return st;
}

char *string(String *st) { return st->str; }

String *string_concat_cstr(Arena *arena, char *s1, char *s2) {
	String *st;
	char *new_str;

	st = (String *)arena_alloc(arena, sizeof(String));
	new_str = (char *)arena_alloc(arena, strlen(s1) + strlen(s2) + 1);

	memcpy(new_str, s1, strlen(s1));
	memcpy(new_str + strlen(s1), s2, strlen(s2));

	new_str[strlen(s1) + strlen(s2)] = '\0';

	st->str = new_str;
	st->length = strlen(s1) + strlen(s2);

	return st;
}

String *string_trim(Arena *arena, String *str) {
	String *st;
	char *new_str;
	int start;
	int end;
	int len;

	start = 0;
	end = string_len(str) - 1;
	len = 0;

	st = (String *)arena_alloc(arena, sizeof(String));

	while (*(str->str + start) == ' ') {
		start++;
	}
	while (end > start && *(str->str + end) == ' ') {
		end--;
	}
	len = (end - start) + 1;
	new_str = (char *)arena_alloc(arena, len + 1);
	memcpy(new_str, str->str + start, len);

	st->str = new_str;
	st->length = len;

	return st;
}

char *read_line(FILE *stream) {
	size_t capacity;
	size_t length;
	char *buffer;
	char *temp;
	int c;

	capacity = 16;
	length = 0;
	buffer = NULL;
	temp = NULL;

	buffer = (char *)m_align_alloc(capacity * sizeof(char));
	if (buffer == NULL) {
		return NULL;
	}

	while ((c = fgetc(stream)) != EOF && c != '\n') {
		/* Check if we need to expand the buffer */
		if (length + 1 >= capacity) {
			capacity *= 2;
			temp = (char *)m_align_realloc(
				buffer, (capacity / 2) * sizeof(char), capacity * sizeof(char));
			if (temp == NULL) {
				m_align_free(buffer);
				return NULL;
			}
			buffer = temp;
		}

		buffer[length++] = (char)c;
	}

	if (length == 0 && c == EOF) {
		m_align_free(buffer);
		return NULL;
	}

	buffer[length] = '\0';

	temp = (char *)m_align_realloc(buffer, capacity * sizeof(char),
								   (length + 1) * sizeof(char));
	if (temp != NULL) {
		buffer = temp;
	}

	return buffer;
}
String *string_get(Arena *arena) {
	char *text;
	String *st;

	fflush(stdout);
	text = read_line(stdin);

	st = string_from(arena, text);
	m_align_free(text);

	return st;
}
