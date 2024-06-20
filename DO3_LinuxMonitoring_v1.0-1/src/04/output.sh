#!/bin/bash

#COLORS=(WHITE RED GREEN BLUE PURPLE BLACK)
COLORS=(39 97 31 32 34 35 30)
BKG_COLORS=(49 107 101 102 44 45 40)

. color_conf
SET_DEFAULT=0
if [[ -z $column1_font_color ]] || [[ -z $column2_font_color ]]\
 || [[ -z $column1_background ]] || [[ -z $column1_background ]]; then 
  SET_DEFAULT=1
fi
if [[ $SET_DEFAULT == 1 ]]; then 
  column1_background=1
  column1_font_color=2
  column2_background=3
  column2_font_color=4
fi
if [[ $column1_background == $column1_font_color ]] ||\
 [[ $column2_background == $column2_font_color ]]; then 
  echo "matched text and background colors";
  exit;
fi

COLOR_TEXT () {
  TEXT_COLOR_KEY=${COLORS["$column1_font_color"]}
  BKG_COLOR_KEY=${BKG_COLORS["$column1_background"]}
  TEXT_COLOR_VALUE=${COLORS["$column2_font_color"]}
  BKG_COLOR_VALUE=${BKG_COLORS["$column2_background"]}
  echo -e "\033[0;"$BKG_COLOR_KEY";"$TEXT_COLOR_KEY"m"$1"\033[0;49m = \033[0;"$BKG_COLOR_VALUE";"$TEXT_COLOR_VALUE"m"$2"\033[0;49m"
}

COLOR_TEXT HOSTNAME $HOSTNAME
COLOR_TEXT TIMEZONE $TIMEZONE
COLOR_TEXT USER $USER 
COLOR_TEXT OS "$OS"
COLOR_TEXT DATE "$DATE"
COLOR_TEXT UPTIME "$UPTIME"
COLOR_TEXT UPTIME_SEC $UPTIME_SEC
COLOR_TEXT IP $IP
COLOR_TEXT MASK $MASK
COLOR_TEXT GATEWAY $GATEWAY
COLOR_TEXT RAM_TOTAL "$RAM_TOTAL GB" 
COLOR_TEXT RAM_USED "$RAM_USED GB" 
COLOR_TEXT RAM_FREE "$RAM_FREE GB" 
COLOR_TEXT SPACE_ROOT "$SPACE_ROOT MB" 
COLOR_TEXT SPACE_ROOT_USED "$SPACE_ROOT_USED MB" 
COLOR_TEXT SPACE_ROOT_FREE "$SPACE_ROOT_FREE MB" 