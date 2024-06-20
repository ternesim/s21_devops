#!/bin/bash

echo
COLOR_NAMES=("dummy" "white" "red" "green" "blue" "purple" "black")
if [[ $SET_DEFAULT == 0 ]]; then 
  echo "Column 1 background = $column1_background ("${COLOR_NAMES[$column1_background]}")"
  echo "Column 1 font color = $column1_font_color ("${COLOR_NAMES[$column1_font_color]}")"
  echo "Column 2 background = $column2_background ("${COLOR_NAMES[$column2_background]}")"
  echo "Column 2 font color = $column2_font_color ("${COLOR_NAMES[$column2_font_color]}")"
else 
  echo "Column 1 background = default ("${COLOR_NAMES[$column1_background]}")"
  echo "Column 1 font color = default ("${COLOR_NAMES[$column1_font_color]}")"
  echo "Column 2 background = default ("${COLOR_NAMES[$column2_background]}")"
  echo "Column 2 font color = default ("${COLOR_NAMES[$column2_font_color]}")"
fi