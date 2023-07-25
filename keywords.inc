switch (text.at(0)) {
case 'i':
if (text.length() <= 1) goto bail;
switch (text.at(1)) {
case 'f':
if (text.length() == 2)return {TokenType::if_, text, current_pos - 1};
if (text.length() <= 2) goto bail;
switch (text.at(2)) {
}
break;
}
break;
case 'e':
if (text.length() <= 1) goto bail;
switch (text.at(1)) {
case 'l':
if (text.length() <= 2) goto bail;
switch (text.at(2)) {
case 's':
if (text.length() <= 3) goto bail;
switch (text.at(3)) {
case 'e':
if (text.length() == 4)return {TokenType::else_, text, current_pos - 1};
if (text.length() <= 4) goto bail;
switch (text.at(4)) {
}
break;
}
break;
}
break;
}
break;
case 'f':
if (text.length() <= 1) goto bail;
switch (text.at(1)) {
case 'o':
if (text.length() <= 2) goto bail;
switch (text.at(2)) {
case 'r':
if (text.length() == 3)return {TokenType::for_, text, current_pos - 1};
if (text.length() <= 3) goto bail;
switch (text.at(3)) {
}
break;
}
break;
case 'n':
if (text.length() == 2)return {TokenType::fn, text, current_pos - 1};
if (text.length() <= 2) goto bail;
switch (text.at(2)) {
}
break;
}
break;
case 'w':
if (text.length() <= 1) goto bail;
switch (text.at(1)) {
case 'h':
if (text.length() <= 2) goto bail;
switch (text.at(2)) {
case 'i':
if (text.length() <= 3) goto bail;
switch (text.at(3)) {
case 'l':
if (text.length() <= 4) goto bail;
switch (text.at(4)) {
case 'e':
if (text.length() == 5)return {TokenType::while_, text, current_pos - 1};
if (text.length() <= 5) goto bail;
switch (text.at(5)) {
}
break;
}
break;
}
break;
}
break;
}
break;
case 'd':
if (text.length() <= 1) goto bail;
switch (text.at(1)) {
case 'o':
if (text.length() == 2)return {TokenType::do_, text, current_pos - 1};
if (text.length() <= 2) goto bail;
switch (text.at(2)) {
}
break;
}
break;
case 'm':
if (text.length() <= 1) goto bail;
switch (text.at(1)) {
case 'o':
if (text.length() <= 2) goto bail;
switch (text.at(2)) {
case 'd':
if (text.length() == 3)return {TokenType::mod, text, current_pos - 1};
if (text.length() <= 3) goto bail;
switch (text.at(3)) {
}
break;
}
break;
}
break;
case 'p':
if (text.length() <= 1) goto bail;
switch (text.at(1)) {
case 'r':
if (text.length() <= 2) goto bail;
switch (text.at(2)) {
case 'o':
if (text.length() <= 3) goto bail;
switch (text.at(3)) {
case 'c':
if (text.length() == 4)return {TokenType::proc, text, current_pos - 1};
if (text.length() <= 4) goto bail;
switch (text.at(4)) {
}
break;
}
break;
}
break;
}
break;
}
bail: 
