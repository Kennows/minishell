CFILES = main.c lexing.c parsing.c redirections.c ft_token_split.c utils.c heredoc.c syntax.c expand.c

LIBSRC = libft/ft_isalpha.c libft/ft_isdigit.c libft/ft_isalnum.c libft/ft_isascii.c libft/ft_isprint.c \
		 libft/ft_strlen.c libft/ft_memset.c libft/ft_bzero.c libft/ft_memcpy.c libft/ft_memmove.c \
		 libft/ft_strlcpy.c libft/ft_strlcat.c libft/ft_toupper.c libft/ft_tolower.c libft/ft_strchr.c \
		 libft/ft_strrchr.c libft/ft_strncmp.c libft/ft_memchr.c libft/ft_memcmp.c libft/ft_strnstr.c \
		 libft/ft_atoi.c libft/ft_calloc.c libft/ft_strdup.c libft/ft_substr.c libft/ft_strjoin.c \
		 libft/ft_strtrim.c libft/ft_split.c libft/ft_itoa.c libft/ft_strmapi.c libft/ft_striteri.c \
		 libft/ft_putchar_fd.c libft/ft_putstr_fd.c libft/ft_putendl_fd.c libft/ft_putnbr_fd.c \
		 libft/ft_printf.c libft/put_functions.c libft/put_functions2.c libft/ft_lstnew_bonus.c \
		 libft/ft_lstadd_front_bonus.c libft/ft_lstsize_bonus.c libft/ft_lstlast_bonus.c \
		 libft/ft_lstadd_back_bonus.c libft/ft_lstdelone_bonus.c libft/ft_lstclear_bonus.c \
		 libft/ft_lstiter_bonus.c libft/ft_lstmap_bonus.c

LIBO = $(LIBSRC:.c=.o)

OFILES = $(CFILES:.c=.o)

LIB = libft/libft.a

LIBDIR = libft

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = minishell

all: $(NAME)

$(LIB): $(LIBO)
	$(MAKE) -C $(LIBDIR)

$(NAME): $(LIB) $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) -lreadline -o $(NAME) $(LIB)

clean:
	rm -f $(OFILES)
	$(MAKE) -C $(LIBDIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBDIR) fclean

re: fclean all

.PHONY: all clean fclean re
