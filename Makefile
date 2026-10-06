# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akunegel <akunegel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/13 14:59:11 by akunegel          #+#    #+#              #
#    Updated: 2023/09/21 14:24:46 by akunegel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Reset
Color_Off=\033[0m       # Text Reset

# Regular Colors
Black='\033[0;30m'        # Black
Red='\033[0;31m'          # Red
Green=\033[0;32m
Yellow='\033[0;33m'       # Yellow
Blue='\033[0;34m'         # Blue
Purple='\033[0;35m'       # Purple
Cyan='\033[0;36m'         # Cyan
White=\033[0;37m        # White

# Bold
BBlack=\033[1;30m
BRed=\033[1;31m
BGreen=\033[1;32m
BYellow=\033[1;33m
BBlue=\033[1;34m
BPurple=\033[1;35m
BCyan=\033[1;36m
BWhite=\033[1;37m

CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRC = src/nm.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = ft_nm

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(EXECUTABLE)
	@echo "${BWhite}push_swap : $(BGreen)[OK✓]${Color_Off}"


%.o: %.c so_long.h
	@$(CC) $(CFLAGS) -c $< -o @$

clean:
	@rm -f $(OBJ)
	@echo "${BWhite}Deleted Files : $(BGreen)[OK✓]${Color_Off}"

fclean: clean
	@rm -f $(EXECUTABLE)
	@echo "${BWhite}Deleted Executable : $(BGreen)[OK✓]${Color_Off}"

re: fclean all