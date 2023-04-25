PROG=	testfs
SRCS=	testfs.c
 
DPADD+=	${LIBVTREEFS} ${LIBFSDRIVER} ${LIBSYS}
LDADD+=	-lvtreefs -lfsdriver -lsys
 
CFLAGS+= -D_NETBSD_SOURCE=1
 
MAN=
 
BINDIR?= /service
 
.include <bsd.prog.mk>
