create table account
(
    account_id   bigint                               not null,
    name         varchar(100)                         null,
    gender       tinyint(1)                           null comment '0男1女',
    birthday     date                                 null,
    selfDesc     varchar(1000)                        null,
    avatar       varchar(255)                         null,
    city         varchar(100)                         null,
    openid       varchar(100)                         null,
    phone        varchar(15)                          null,
    wechat       varchar(100)                         null,
    email        varchar(100)                         null,
    industry     varchar(100)                         null,
    type         tinyint(1) default 1                 null comment '0学生1工作',
    registered   tinyint(1) default 0                 null comment '0未注册1已注册',
    c_time       datetime   default CURRENT_TIMESTAMP null,
    u_time       datetime   default CURRENT_TIMESTAMP null,
    valid_status tinyint(1) default 1                 null comment '1有效0无效',
    constraint account_account_id_uindex
        unique (account_id)
)
    comment '账户表';

alter table account
    add primary key (account_id);

create table education
(
    account_id   bigint                               not null,
    education_id bigint                               not null
        primary key,
    education    varchar(100)                         not null,
    school       varchar(100)                         not null,
    college      varchar(100)                         not null,
    start_time   datetime                             not null,
    end_time     datetime                             not null,
    c_time       datetime   default CURRENT_TIMESTAMP not null,
    u_time       datetime   default CURRENT_TIMESTAMP not null,
    valid_status tinyint(1) default 1                 not null
);

create index education_account_account_id_fk
    on education (account_id);

create table friend
(
    account_id        bigint                               not null,
    friend_account_id bigint                               not null,
    status            tinyint(1)                           not null comment '    stranger(0, "路人"),
    applying(2, "A向B发起好友申请，或者A有待处理的B的好友申请"),
    friend(1, "好友"),
    rejected(3, "被拒绝");',
    c_time            datetime   default CURRENT_TIMESTAMP not null,
    u_time            datetime   default CURRENT_TIMESTAMP not null,
    valid_status      tinyint(1) default 1                 not null
);

create index friend_account_account_id_fk
    on friend (account_id);

create table job
(
    account_id   bigint                               not null,
    job_id       bigint                               not null
        primary key,
    company      varchar(255)                         not null,
    position     varchar(255)                         not null,
    start_time   datetime                             not null,
    ennd_time    datetime                             not null,
    c_time       datetime   default CURRENT_TIMESTAMP not null,
    u_time       datetime   default CURRENT_TIMESTAMP not null,
    valid_status tinyint(1) default 1                 not null
);

create index job_account_account_id_fk
    on job (account_id);

