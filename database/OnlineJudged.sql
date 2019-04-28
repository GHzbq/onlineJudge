-- 这个文件是hustoj的数据库文件
-- 设置服务器与客户端交互的字符集为utf8
set names utf8; 
-- 创建一个 jol 数据库
create database OnlineJudged;
-- 使用刚刚创建的jol数据库
use OnlineJudged;

-- 创建一张编译信息表
CREATE TABLE  `compileinfo` (
  `solution_id` int(11) NOT NULL DEFAULT 0 comment "题目序号",
  `error` text comment "错误信息",
  PRIMARY KEY (`solution_id`) comment "将 solution_id 设置为主键"
) ENGINE=MyISAM DEFAULT CHARSET=utf8 comment "设置引擎为MyISAM 字符集为utf8";

-- -- 创建一张比赛信息表
-- CREATE TABLE  `contest` (
--   `contest_id` int(11) NOT NULL AUTO_INCREMENT comment "比赛ID",
--   `title` varchar(255) DEFAULT NULL comment "比赛标题",
--   `start_time` datetime DEFAULT NULL comment "比赛起始时间",
--   `end_time` datetime DEFAULT NULL comment "比赛结束时间",
--   `defunct` char(1) NOT NULL DEFAULT 'N',
--   `description` text comment "本场比赛的描述",
--   `private` tinyint(4) NOT NULL DEFAULT '0' ,
--   `langmask` int NOT NULL DEFAULT '0' COMMENT 'bits for LANG to mask',
--   `password` CHAR( 16 ) NOT NULL DEFAULT '' comment "密码",
--   PRIMARY KEY (`contest_id`) comment "将比赛id设为主键"
-- ) ENGINE=MyISAM AUTO_INCREMENT=1000 DEFAULT CHARSET=utf8 comment "指定存储引擎为MyISAM,字符集为utf8, 自增长初始值为1000";

-- -- 创建比赛题目表
-- CREATE TABLE  `contest_problem` (
--   `problem_id` int(11) NOT NULL DEFAULT 0 comment "题目ID",
--   `contest_id` int(11) DEFAULT NULL comment "比赛ID",
--   `title` char(200) NOT NULL DEFAULT '' comment "题目名称",
--   `num` int(11) NOT NULL DEFAULT 0 comment ""
-- ) ENGINE=MyISAM DEFAULT CHARSET=utf8 comment "指定存储引擎为 MyISAM, 字符集为 utf8";


-- 创建登录信息表，浏览器端提交的请求
CREATE TABLE `loginlog` (
  `user_id` varchar(48) NOT NULL DEFAULT '' comment "用户提交账号",
  `password` varchar(40) DEFAULT NULL comment "用户提交的密码",
  `ip` varchar(46) DEFAULT NULL comment "本次提交用户的IP地址",
  `time` datetime DEFAULT NULL comment "提交的时间",
  KEY `user_log_index` (`user_id`,`time`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- CREATE TABLE  `mail` (
--   `mail_id` int(11) NOT NULL AUTO_INCREMENT,
--   `to_user` varchar(48) NOT NULL DEFAULT '',
--   `from_user` varchar(48) NOT NULL DEFAULT '',
--   `title` varchar(200) NOT NULL DEFAULT '',
--   `content` text,
--   `new_mail` tinyint(1) NOT NULL DEFAULT '1',
--   `reply` tinyint(4) DEFAULT '0',
--   `in_date` datetime DEFAULT NULL,
--   `defunct` char(1) NOT NULL DEFAULT 'N',
--   PRIMARY KEY (`mail_id`),
--   KEY `uid` (`to_user`)
-- ) ENGINE=MyISAM AUTO_INCREMENT=1013 DEFAULT CHARSET=utf8;

-- CREATE TABLE  `news` (
--   `news_id` int(11) NOT NULL AUTO_INCREMENT,
--   `user_id` varchar(48) NOT NULL DEFAULT '',
--   `title` varchar(200) NOT NULL DEFAULT '',
--   `content` text NOT NULL,
--   `time` datetime NOT NULL DEFAULT '2016-05-13 19:24:00',
--   `importance` tinyint(4) NOT NULL DEFAULT '0',
--   `defunct` char(1) NOT NULL DEFAULT 'N',
--   PRIMARY KEY (`news_id`)
-- ) ENGINE=MyISAM AUTO_INCREMENT=1004 DEFAULT CHARSET=utf8;


-- CREATE TABLE  `privilege` (
--   `user_id` char(48) NOT NULL DEFAULT '',
--   `rightstr` char(30) NOT NULL DEFAULT '',
--   `defunct` char(1) NOT NULL DEFAULT 'N'
-- ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- 对一个具体OJ题目的描述
CREATE TABLE  `problem` (
  `problem_id` int(11) NOT NULL AUTO_INCREMENT comment "题目ID 自增长",
  `title` varchar(200) NOT NULL DEFAULT '' comment "题目标题",
  `description` text comment "题目的描述",
  `input` text comment "输入描述",
  `output` text comment "输出描述",
  `sample_input` text comment "样例输入",
  `sample_output` text comment "样例输出",
  `spj` char(1) NOT NULL DEFAULT '0' ,
  `hint` text,
  `source` varchar(100) DEFAULT NULL,
  `in_date` datetime DEFAULT NULL,
  `time_limit` int(11) NOT NULL DEFAULT 0 comment "本题的时间限制",
  `memory_limit` int(11) NOT NULL DEFAULT 0 comment "本题的内存限制",
  `defunct` char(1) NOT NULL DEFAULT 'N',
  `accepted` int(11) DEFAULT '0' comment "通过人数",
  `submit` int(11) DEFAULT '0' comment "提交人数",
  `solved` int(11) DEFAULT '0' comment "解决人数",
  PRIMARY KEY (`problem_id`) comment  "设置 题目ID 为主键"
) ENGINE=MyISAM AUTO_INCREMENT=1000 DEFAULT CHARSET=utf8 comment "设置存储引擎为MyISAM,字符集默认 utf8 , 自增长起始值为1000";

-- CREATE TABLE  `reply` (
--   `rid` int(11) NOT NULL AUTO_INCREMENT,
--   `author_id` varchar(48) NOT NULL,
--   `time` datetime NOT NULL DEFAULT '2016-05-13 19:24:00',
--   `content` text NOT NULL,
--   `topic_id` int(11) NOT NULL,
--   `status` int(2) NOT NULL DEFAULT '0',
--   `ip` varchar(46) NOT NULL,
--   PRIMARY KEY (`rid`),
--   KEY `author_id` (`author_id`)
-- ) ENGINE=MyISAM DEFAULT CHARSET=utf8;


-- CREATE TABLE  `sim` (
--   `s_id` int(11) NOT NULL,
--   `sim_s_id` int(11) DEFAULT NULL,
--   `sim` int(11) DEFAULT NULL,
--   PRIMARY KEY (`s_id`)
-- ) ENGINE=MyISAM DEFAULT CHARSET=utf8;


CREATE TABLE  `solution` (
  `solution_id` int(11) NOT NULL AUTO_INCREMENT,
  `problem_id` int(11) NOT NULL DEFAULT 0,
  `user_id` char(48) NOT NULL,
  `time` int(11) NOT NULL DEFAULT 0,
  `memory` int(11) NOT NULL DEFAULT 0,
  `in_date` datetime NOT NULL DEFAULT '2016-05-13 19:24:00',
  `result` smallint(6) NOT NULL DEFAULT '0',
  `language` INT UNSIGNED NOT NULL DEFAULT '0',
  `ip` char(46) NOT NULL,
  `contest_id` int(11) DEFAULT NULL,
  `valid` tinyint(4) NOT NULL DEFAULT '1',
  `num` tinyint(4) NOT NULL DEFAULT '-1',
  `code_length` int(11) NOT NULL DEFAULT 0,
  `judgetime` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `pass_rate` DECIMAL(3,2) UNSIGNED NOT NULL DEFAULT 0,
  `lint_error` int UNSIGNED NOT NULL DEFAULT 0,
  `judger` CHAR(16) NOT NULL DEFAULT 'LOCAL',
  PRIMARY KEY (`solution_id`),
  KEY `uid` (`user_id`),
  KEY `pid` (`problem_id`),
  KEY `res` (`result`),
  KEY `cid` (`contest_id`)
) ENGINE=MyISAM row_format=fixed AUTO_INCREMENT=1001 DEFAULT CHARSET=utf8;

-- 题目id为solution_id的题目源代码
CREATE TABLE  `source_code` (
  `solution_id` int(11) NOT NULL,
  `source` text NOT NULL,
  PRIMARY KEY (`solution_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
-- 用户提交的代码
create table source_code_user like source_code;

-- CREATE TABLE  `topic` (
--   `tid` int(11) NOT NULL AUTO_INCREMENT,
--   `title` varbinary(60) NOT NULL,
--   `status` int(2) NOT NULL DEFAULT '0',
--   `top_level` int(2) NOT NULL DEFAULT '0',
--   `cid` int(11) DEFAULT NULL,
--   `pid` int(11) NOT NULL,
--   `author_id` varchar(48) NOT NULL,
--   PRIMARY KEY (`tid`),
--   KEY `cid` (`cid`,`pid`)
-- ) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE  `users` (
  `user_id` varchar(48) NOT NULL DEFAULT '',
  `email` varchar(100) DEFAULT NULL,
  `submit` int(11) DEFAULT '0',
  `solved` int(11) DEFAULT '0',
  `defunct` char(1) NOT NULL DEFAULT 'N',
  `ip` varchar(46) NOT NULL DEFAULT '',
  `accesstime` datetime DEFAULT NULL,
  `volume` int(11) NOT NULL DEFAULT '1',
  `language` int(11) NOT NULL DEFAULT '1',
  `password` varchar(32) DEFAULT NULL,
  `reg_time` datetime DEFAULT NULL,
  `nick` varchar(20) NOT NULL DEFAULT '',
  `school` varchar(20) NOT NULL DEFAULT '',
  PRIMARY KEY (`user_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE `online` (
  `hash` varchar(32) collate utf8_unicode_ci NOT NULL,
  `ip` varchar(46) character set utf8 NOT NULL default '',
  `ua` varchar(255) character set utf8 NOT NULL default '',
  `refer` varchar(255) collate utf8_unicode_ci default NULL,
  `lastmove` int(10) NOT NULL,
  `firsttime` int(10) default NULL,
  `uri` varchar(255) collate utf8_unicode_ci default NULL,
  PRIMARY KEY  (`hash`),
  UNIQUE KEY `hash` (`hash`)
) ENGINE=MEMORY DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

CREATE TABLE  `runtimeinfo` (
  `solution_id` int(11) NOT NULL DEFAULT 0,
  `error` text,
  PRIMARY KEY (`solution_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- 客户输入
CREATE TABLE  `custominput` (
  `solution_id` int(11) NOT NULL DEFAULT 0,
  `input_text` text,
  PRIMARY KEY (`solution_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--CREATE TABLE  `printer` (
--  `printer_id` int(11) NOT NULL AUTO_INCREMENT,
--  `user_id` char(48) NOT NULL,
--  `in_date` datetime NOT NULL DEFAULT '2018-03-13 19:38:00',
--  `status` smallint(6) NOT NULL DEFAULT '0',
--  `worktime` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
--  `printer` CHAR(16) NOT NULL DEFAULT 'LOCAL',
--  `content` text NOT NULL ,
--  PRIMARY KEY (`printer_id`)
--) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
--
--CREATE TABLE  `balloon` (
--  `balloon_id` int(11) NOT NULL AUTO_INCREMENT,
--  `user_id` char(48) NOT NULL,
--  `sid` int(11) NOT NULL ,
--  `cid` int(11) NOT NULL ,
--  `pid` int(11) NOT NULL ,
--  `status` smallint(6) NOT NULL DEFAULT '0',
--  PRIMARY KEY (`balloon_id`)
--) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
--
--delimiter //
--drop trigger if exists simfilter//
--create trigger simfilter
--before insert on sim
--for each row
--begin
-- declare new_user_id varchar(64);
-- declare old_user_id varchar(64);
-- select user_id from solution where solution_id=new.s_id into new_user_id;
-- select user_id from solution where solution_id=new.sim_s_id into old_user_id;
-- if old_user_id=new_user_id then
--	set new.s_id=0;
-- end if;
-- 
--end;//
--delimiter ;
