find -iname '*.h' -o -iname '*.hpp' -o -iname '*.c' -o -iname '*.cpp' -o -name '*.glsl' -o -name '*.tcc' | entr -cs "clear && $@"
