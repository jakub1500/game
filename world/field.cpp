#include "field.h"

Field::Field(void) : field_image(' '){}

char Field::get_image(void) {
    return field_image;
}

void Field::set_image(char c) {
    field_image = c;
}

bool Field::accessible(void) {
    return field_image == ' ';
}