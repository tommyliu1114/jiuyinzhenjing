/*
    Pirate Bulletin Board System
    Copyright (C) 1990, Edward Luke, lush@Athena.EE.MsState.EDU
    Eagles Bulletin Board System
    Copyright (C) 1992, Raymond Rocker, rocker@rock.b11.ingr.com
                        Guy Vega, gtvega@seabass.st.usm.edu
                        Dominic Tynes, dbtynes@seabass.st.usm.edu
    Firebird Bulletin Board System
    Copyright (C) 1996, Hsien-Tsung Chang, Smallpig.bbs@bbs.cs.ccu.edu.tw
                        Peng Piaw Foong, ppfoong@csie.ncu.edu.tw

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 1, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/
/*
$Id: delete.c,v 1.6 1999/09/07 03:30:32 edwardc Exp $
*/

#include "bbs.h"
#ifdef WITHOUT_ADMIN_TOOLS
#define kick_user
#endif

int
offline()
{
	char    buf[STRLEN];
	modify_user_mode(OFFLINE);
	clear();
	if (HAS_PERM(PERM_SYSOP) || HAS_PERM(PERM_BOARDS) || HAS_PERM(PERM_ADMINMENU)
		|| HAS_PERM(PERM_SEEULEVELS)) {
		move(1, 0);
		prints("\n\n您有重任在身, 不能随便自杀啦!!\n");
		pressreturn();
		clear();
		return;
	}
	if (currentuser.stay < 86400) {
		move(1, 0);
		prints("\n\n对不起, 您还未够资格执行此命令!!\n");
		prints("请 mail 给 SYSOP 说明自杀原因, 谢谢。\n");
		pressreturn();
		clear();
		return;
	}
	getdata(1, 0, "请输入你的密码: ", buf, PASSLEN, NOECHO, YEA);
	if (*buf == '\0' || !checkpasswd(currentuser.passwd, buf)) {
		prints("\n\n很抱歉, 您输入的密码不正确。\n");
		pressreturn();
		clear();
		return;
	}
	getdata(3, 0, "请问你叫什麽名字? ", buf, NAMELEN, DOECHO, YEA);
	if (*buf == '\0' || strcmp(buf, currentuser.realname)) {
		prints("\n\n很抱歉, 我并不认识你。\n");
		pressreturn();
		clear();
		return;
	}
	clear();
	move(1, 0);
	prints("[1;5;31m警告[0;1;31m： 自杀後, 您将无法再用此帐号进入本站！！");
	prints("\n\n\n[1;32m但帐号要在 30 天後才会删除。好难过喔 :( .....[m\n\n\n");
	if (askyn("你确定要离开这个大家庭", NA, NA) == 1) {
		clear();
		currentuser.userlevel = 0;
		substitute_record(PASSFILE, &currentuser, sizeof(struct userec), usernum);
		mail_info();
		modify_user_mode(OFFLINE);
		kick_user(&uinfo);
		exit(0);
	}
}
getuinfo(fn)
FILE   *fn;
{
	int     num;
	char    buf[40];
	fprintf(fn, "\n\n他的代号     : %s\n", currentuser.userid);
	fprintf(fn, "他的昵称     : %s\n", currentuser.username);
	fprintf(fn, "真实姓名     : %s\n", currentuser.realname);
	fprintf(fn, "居住住址     : %s\n", currentuser.address);
	fprintf(fn, "电子邮件信箱 : %s\n", currentuser.email);
	fprintf(fn, "真实 E-mail  : %s\n", currentuser.reginfo);
	fprintf(fn, "Ident 资料   : %s\n", currentuser.ident);
	fprintf(fn, "帐号建立日期 : %s", ctime(&currentuser.firstlogin));
	fprintf(fn, "最近光临日期 : %s", ctime(&currentuser.lastlogin));
	fprintf(fn, "最近光临机器 : %s\n", currentuser.lasthost);
	fprintf(fn, "上站次数     : %d 次\n", currentuser.numlogins);
	fprintf(fn, "文章数目     : %d\n", currentuser.numposts);
	fprintf(fn, "上站总时数   : %d 小时 %d 分钟\n",
		currentuser.stay / 3600, (currentuser.stay / 60) % 60);
	strcpy(buf, "bTCPRp#@XWBA#VS-DOM-F012345678");
	for (num = 0; num < 30; num++)
		if (!(currentuser.userlevel & (1 << num)))
			buf[num] = '-';
	buf[num] = '\0';
	fprintf(fn, "使用者权限   : %s\n\n", buf);
}
mail_info()
{
	FILE   *fn;
	time_t  now;
	char    filename[STRLEN];
	now = time(0);
	sprintf(filename, "tmp/suicide.%s", currentuser.userid);
	if ((fn = fopen(filename, "w")) != NULL) {
		fprintf(fn, "[1m%s[m 已经在 [1m%24.24s[m 登记自杀了，以下是他的资料，请保留...", currentuser.userid
			,ctime(&now));
		getuinfo(fn);
		fclose(fn);
		postfile(filename, "syssecurity", "登记自杀通知(30天後生效)...", 2);
		unlink(filename);
	}
	if ((fn = fopen(filename, "w")) != NULL) {
		fprintf(fn, "大家好,\n\n");
		fprintf(fn, "我是 %s (%s)。我己经登记在30天後离开这里了。\n\n",
			currentuser.userid, currentuser.username);
		fprintf(fn, "我不会更不可能忘记自 %s以来在本站 %d 次 login 中总共 %d 分钟逗留期间的点点滴滴。\n",
			ctime(&currentuser.firstlogin), currentuser.numlogins, currentuser.stay / 60);
		fprintf(fn, "请我的好友把 %s 从你们的好友名单中拿掉吧。因为我己经没有权限再上站了!\n\n",
			currentuser.userid);
		fprintf(fn, "或许有朝一日我会回来的。 珍重!! 再见!!\n\n\n");
		fprintf(fn, "%s 於 %24.24s 留.\n\n", currentuser.userid, ctime(&now));
		fclose(fn);
		postfile(filename, "notepad", "登记自杀留言...", 2);
		unlink(filename);
	}
}
