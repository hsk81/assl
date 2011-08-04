/* $assl$ */
/*
 * Copyright (c) 2011 Conformal Systems LLC <info@conformal.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>

#include "assl.h"
#include "assl_internal.h"

int
assl_is_nonblock(struct assl_context *c, int s)
{
	int	r, rv = -1;

	r = fcntl(s, F_GETFL, 0);
	if (r < 0)
		ERROR_OUT(ERR_LIBC, done);

	rv = 0;
	if (r & O_NONBLOCK)
		rv = 1;
done:
	return (rv);
}

int
assl_set_nonblock(int fd)
{
	int			val, rv = 1;

	val = fcntl(fd, F_GETFL, 0);
	if (val < 0)
		goto done;

	if (val & O_NONBLOCK)
		return (0);

	val |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, val) == -1)
		goto done;

	rv = 0;
done:
	return (rv);
}

int
assl_set_keepalive(int fd)
{
	int			val = 1;

	if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val)) == -1)
		return (-1);

	return (0);
}

void
assl_set_tos(int fd, int flags)
{
	int	tos;

	if (flags & ASSL_F_LOWDELAY) {
		tos = IPTOS_LOWDELAY;
		setsockopt(fd, IPPROTO_IP, IP_TOS, &tos, sizeof(tos));
	}
	if (flags & ASSL_F_THROUGHPUT) {
		tos = IPTOS_THROUGHPUT;
		setsockopt(fd, IPPROTO_IP, IP_TOS, &tos, sizeof(tos));
	}
}
