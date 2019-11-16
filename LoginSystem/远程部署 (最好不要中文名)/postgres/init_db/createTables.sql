
create table tb_userAccountInformation(
    userId serial primary key,
    userPhone varchar(16) unique not null,
    userEmail varchar(30) unique not null,
    userPassword varchar(20) not null,
    userType int not null,
    userRegisterTime bigint not null,
    userLastLoginTime bigint not null,
    reserved1	varchar(1),
    reserved2	varchar(1)

);


create table tb_userPersonalInformation(
    userId integer primary key references tb_userAccountInformation(userId),
    userPhoto varchar(80),	
    userName varchar(20),
    userSex  varchar(10),
    userContactPhone varchar(16),
    userContactEmail varchar(30),
    userBirthday bigint,
    reserved1	varchar(1),
    reserved2	varchar(1)
);