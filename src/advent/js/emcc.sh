#source "/opt/emscripten/emsdk/emsdk_env.sh"
emcc ../gcc/*.c -funsigned-char -sASYNCIFY -o advent.js
