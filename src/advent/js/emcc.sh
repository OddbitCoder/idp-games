#source "/opt/emscripten/emsdk/emsdk_env.sh"
emcc ../gcc/*.c -O3 -funsigned-char -sASYNCIFY -o advent.js
