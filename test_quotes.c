#include <stdio.h>
#include <stdlib.h>
# include <limits.h>

size_t	ft_strlen(const char *str)
{
	const char	*start = str;

	while (*str != '\0')
		str++;
	return (str - start);
}

size_t	ft_strlcpy(char *d, const char *src, size_t size)
{
	const char	*s = src;

	while (size > 1 && *s != '\0')
	{
		*d++ = *s++;
		size--;
	}
	if (size > 0)
		*d = '\0';
	while (*s)
		s++;
	return (s - src);
}
void	ft_bzero(void *s, size_t n)
{
	unsigned char *const	ptr = (unsigned char *)s;

	while (n--)
		*(ptr + n) = 0;
}
void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total_size;
	void	*ptr;

	if (!nmemb || !size)
		return (malloc(0));
	total_size = nmemb * size;
	if (total_size / nmemb != size)
		return (NULL);
	ptr = malloc(total_size);
	if (ptr)
		ft_bzero(ptr, total_size);
	return (ptr);
}

char	*ft_strdup(const char *src)
{
	char			*dup;
	const size_t	size = ft_strlen(src) + 1;

	dup = (char *)ft_calloc(sizeof(char), size);
	if (!dup)
		return (NULL);
	ft_strlcpy(dup, src, size);
	return (dup);
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char *const	dest = dst;
	const unsigned char		*source = src;
	size_t					it;

	it = 0;
	if (dst > src)
	{
		while (len--)
			*(dest + len) = *(source + len);
	}
	else if (dst < src)
	{
		while (it < len)
		{
			*(dest + it) = *(source + it);
			it++;
		}
	}
	return (dst);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	u_int64_t		*dest;
	const u_int64_t	*source;
	size_t			it;
	const size_t	reminder = n % sizeof(u_int64_t);

	dest = dst;
	source = src;
	n /= sizeof(u_int64_t);
	if (dst == src)
		return (dst);
	it = 0;
	while (it < n)
	{
		*dest++ = *source++;
		it++;
	}
	it = 0;
	while (it < reminder)
	{
		*((char *)dest + it) = *((char *)source + it);
		it++;
	}
	return (dst);
}


char    *remove_quotes(char **str)
{
    char    *cur_byte;
    char    *last_byte;
    char    quote_type;

    cur_byte = *str;
    last_byte = cur_byte + ft_strlen(*str);
    quote_type = 0;
    while (*cur_byte)
    {
		if ((quote_type == 0 && (*cur_byte == '\'' || *cur_byte == '"'))
			|| (quote_type == *cur_byte))
		{
			if (quote_type == 0)
				quote_type = *cur_byte;
			else
				quote_type = *cur_byte;
			ft_memmove(cur_byte, cur_byte + 1, last_byte - cur_byte);
			last_byte--;
		}
        cur_byte++;
    }
    return (*str);
}
#define T_BUFFER_SIZE 42
#define SUCCESS 0

typedef struct s_ch_buffer
{
	char	*buf;
	size_t	sz;
	size_t	capacity;
}		t_ch_buffer;

int	buffer_extend(t_ch_buffer *buffer)
{
	char	*new;
	char	*to_free;

	to_free = buffer->buf;
	new = ft_calloc(sizeof(char), buffer->capacity + T_BUFFER_SIZE + 1);
	if (!new)
		return (!SUCCESS);
	ft_memcpy(new, buffer->buf, buffer->sz + 1);
	buffer->capacity = buffer->capacity + T_BUFFER_SIZE + 1;
	if (to_free)
		free(to_free);
	return (SUCCESS);
}

size_t	ft_strlcat(char *d, const char *src, size_t size)
{
	size_t			len;

	len = 0;
	while (len < size && d[len])
		len++;
	if (d[len] != '\0')
		return (size + ft_strlen(src));
	return (len + ft_strlcpy(d + len, src, size - len));
}

int	buffer_add(t_ch_buffer *buffer, char *appendix)
{
	size_t	append_len;

	append_len = ft_strlen(appendix);
	while ((buffer->sz + append_len) >= buffer->capacity)
	{
		if (buffer_extend(buffer) != SUCCESS)
			return (!SUCCESS);
	}
	ft_strlcat(buffer->buf + buffer->sz, appendix, buffer->capacity);
	return (SUCCESS);
}

int	main(void)
{
	t_ch_buffer	buf;

	ft_bzero(&buf, sizeof(t_ch_buffer));
	buffer_add(&buf, "abc");

	printf("buf: %s\n", buf.buf);
	return (0);
}
