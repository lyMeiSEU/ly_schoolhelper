package cn.edu.seu.alumni_server.dataSync;

import cn.edu.seu.alumni_server.common.Utils;
import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.dao.entity.Account;
import cn.edu.seu.alumni_server.dao.entity.Job;
import cn.edu.seu.alumni_server.dao.mapper.AccountMapper;
import cn.edu.seu.alumni_server.dao.mapper.JobMapper;
import cn.edu.seu.alumni_server.dataSync.entity.*;
import cn.edu.seu.alumni_server.dataSync.mapper.*;
import com.github.pagehelper.Page;
import com.github.pagehelper.PageHelper;
import com.google.gson.Gson;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import tk.mybatis.mapper.entity.Example;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.List;


/**
 * 数据库同步代码，v2版本做了数据库表的大调整
 * 主要是把v1版本db数据（cAuth），同步到新的db(alumnidb_dev)
 */
@RestController
@RequestMapping("/data")
@Slf4j
public class DataSyncController {

    // V1
    @Autowired
    @Qualifier(value = "EducationMapperV1")
    EducationMapper educationMapperV1;

    @Autowired
    @Qualifier(value = "FriendMapperV1")
    FriendMapper friendMapperV1;

    @Autowired
    PersonalMapper personalMapper;

    @Autowired
    PersonalinforMapper personalinforMapper;

    @Autowired
    WorkMapper workMapper;

    //v2
    @Autowired
    AccountMapper accountMapper;

    @Autowired
    cn.edu.seu.alumni_server.dao.mapper.EducationMapper educationMapperV2;

    @Autowired
    JobMapper jobMapper;

    @Autowired
    cn.edu.seu.alumni_server.dao.mapper.FriendMapper friendMapperV2;

    @GetMapping("/sync")
    public WebResponse dataSync() {
//        personal2account();
//        personalInfor2Account();
//        educationSync();
//        workSync();

//        friendSync();

        return new WebResponse();
    }

    void friendSync() {
        PageHelper.startPage(1, 1);

        List<Friend> res = friendMapperV1.selectAll();

        long total = ((Page) res).getTotal();
        int pageNum = 0;
        int pageSize = 10;

        while (pageNum * pageSize < total) {
            pageNum++;

            PageHelper.startPage(pageNum, pageSize);
            List<Friend> resTemp = friendMapperV1.selectAll();

            resTemp.forEach(friend -> {
                cn.edu.seu.alumni_server.dao.entity.Friend friendV2 =
                        new cn.edu.seu.alumni_server.dao.entity.Friend();

                Example example = new Example(Account.class);
                example.createCriteria().andEqualTo("openid", friend.getOpenid());
                Account account1 = accountMapper.selectOneByExample(example);

                Example example2 = new Example(Account.class);
                example2.createCriteria().andEqualTo("openid", friend.getFriendid());
                Account account2 = accountMapper.selectOneByExample(example2);

                if (account1 != null && account2 != null) {

                    friendV2.setAccountId(account1.getAccountId());
                    friendV2.setFriendAccountId(account2.getAccountId());

                    Integer status = 0;
                    switch (friend.getState()) {
                        case "0":
                            status = 0;
                            break;
                        case "1":
                            status = 2;
                            break;
                        case "2":
                            status = 1;
                            break;
                        case "3":
                            status = 1;
                            break;
                        case "4":
                            status = 0;
                            break;
                        case "5":
                            status = 2;
                            break;
                        default:
                            status = 0;
                    }


                    friendV2.setStatus(status);

                    try {
                        friendMapperV2.insertSelective(friendV2);

                        cn.edu.seu.alumni_server.dao.entity.Friend friendV22 =
                                new cn.edu.seu.alumni_server.dao.entity.Friend();

                        friendV22.setStatus(friendV2.getStatus());
                        friendV22.setAccountId(account2.getAccountId());
                        friendV22.setFriendAccountId(account1.getAccountId());
                        friendMapperV2.insertSelective(friendV22);
                    } catch (Exception e) {
                        log.info(friend.toString());
                        e.printStackTrace();
                    }
                }

            });
        }

    }

    void educationSync() {

        PageHelper.startPage(1, 1);
        List<Education> res = educationMapperV1.selectAll();

        long total = ((Page) res).getTotal();
        int pageNum = 0;
        int pageSize = 10;

        while (pageNum * pageSize < total) {
            pageNum++;

            PageHelper.startPage(pageNum, pageSize);
            List<Education> resTemp = educationMapperV1.selectAll();

            resTemp.forEach(education -> {
                cn.edu.seu.alumni_server.dao.entity.Education educationV2 =
                        new cn.edu.seu.alumni_server.dao.entity.Education();

                Example example = new Example(Account.class);
                example.createCriteria().andEqualTo("openid", education.getOpenid());
                Account account = accountMapper.selectOneByExample(example);

                if (account != null && account.getAccountId() != null) {
                    educationV2.setEducationId(Utils.generateId());
                    educationV2.setAccountId(account.getAccountId());
                    educationV2.setCollege(education.getDepartment());
                    educationV2.setSchool(education.getSchool());
                    try {
                        educationV2.setStartTime(
                                new SimpleDateFormat("yyyy")
                                        .parse(education.getStartYear())
                                        .getTime()
                        );

                        educationV2.setEndTime(
                                new SimpleDateFormat("yyyy")
                                        .parse(education.getStartYear())
                                        .getTime()
                        );
                    } catch (Throwable e) {
                        e.printStackTrace();
                    }
                    educationV2.setEducation(education.getBackground());

                    educationMapperV2.insertSelective(educationV2);
                } else {
                    log.info(education.getOpenid() + "education 转换报错,account {}",
                            new Gson().toJson(account));
                }
            });
        }

    }

    void workSync() {

        PageHelper.startPage(1, 1);
        List<Work> res = workMapper.selectAll();

        long total = ((Page) res).getTotal();
        int pageNum = 0;
        int pageSize = 10;

        while (pageNum * pageSize < total) {
            pageNum++;

            PageHelper.startPage(pageNum, pageSize);
            List<Work> resT = workMapper.selectAll();

            resT.forEach(work -> {

                Job job = new Job();

                Example example = new Example(Account.class);
                example.createCriteria().andEqualTo("openid", work.getOpenid());
                Account account = accountMapper.selectOneByExample(example);
                if (account != null && account.getAccountId() != null) {
                    job.setAccountId(account.getAccountId());
                    job.setJobId(Utils.generateId());
                    job.setCompany(work.getCompany());
                    job.setPosition(work.getJob());
                    try {
                        if (!StringUtils.isEmpty(work.getStartYear())) {
                            job.setStartTime(
                                    new SimpleDateFormat("yyyy")
                                            .parse(work.getStartYear())
                                            .getTime());
                        }
                        if (!StringUtils.isEmpty(work.getEndYear())) {
                            job.setStartTime(
                                    new SimpleDateFormat("yyyy")
                                            .parse(work.getEndYear())
                                            .getTime()
                            );
                        }

                    } catch (Throwable e) {
                        e.printStackTrace();
                        log.info(work.getOpenid() + "work 转换失败");

                    }
                    jobMapper.insertSelective(job);
                } else {
                    log.info(work.getOpenid() + "work 转换失败，account {}",
                            new Gson().toJson(account));
                }

            });
        }
    }

    void personal2account() {

        PageHelper.startPage(1, 1);
        List<Personal> res = personalMapper.selectAll();

        long total = ((Page) res).getTotal();
        int pageNum = 0;
        int pageSize = 10;

        while (pageNum * pageSize < total) {
            pageNum++;

            PageHelper.startPage(pageNum, pageSize);
            List<Personal> resTemp = personalMapper.selectAll();

            resTemp.forEach(personal -> {
                Account account = new Account();
                account.setAccountId(Utils.generateId());

                account.setOpenid(personal.getOpenid());
                account.setWechat(personal.getWechat());
                account.setPhone(personal.getPhone());
                account.setEmail(personal.getEmail());

                try {
                    if (accountMapper.selectByPrimaryKey(account.getAccountId()) == null) {
                        return;
                    }
                    if (accountMapper.insertSelective(account) != 1) {
                        log.error("account 插入失败");
                    }
                } catch (Throwable t) {
                    log.info(t.getMessage());
                }
            });
        }

    }

    void personalInfor2Account() {

        PageHelper.startPage(1, 1);
        List<Personalinfor> res = personalinforMapper.selectAll();

        long total = ((Page) res).getTotal();
        int pageNum = 0;
        int pageSize = 10;
        Integer count = 0;

        while (pageNum * pageSize < total) {
            pageNum++;
            System.out.println(count++);

            PageHelper.startPage(pageNum, pageSize);
            List<Personalinfor> resTemp = personalinforMapper.selectAll();

            resTemp.forEach(personalinfor -> {

                Account account = null;
                try {
                    Example example = new Example(Account.class);
                    example.createCriteria().andEqualTo("openid", personalinfor.getOpenid());
                    account = accountMapper.selectOneByExample(example);

                    if (account == null) {
                        return;
                    }
                    account.setName(personalinfor.getRealName());
                    account.setGender(
                            Integer.parseInt(personalinfor.getGender())
                    );
                } catch (Throwable e) {
                    e.printStackTrace();
                }
                account.setSelfDesc(personalinfor.getDescr());
                account.setCity(personalinfor.getCity());
                account.setAvatar(personalinfor.getHeadUrl());
                if (!StringUtils.isEmpty(personalinfor.getBirth())) {
                    try {
                        account.setBirthday(
                                new SimpleDateFormat("yyyy-MM-dd")
                                        .parse(personalinfor.getBirth()).getTime());
                    } catch (ParseException e) {
                        e.printStackTrace();
                    }
                }

                try {
                    if (accountMapper.updateByPrimaryKeySelective(account) != 1) {
                        log.error("account 插入失败");
                    }
                } catch (Throwable t) {
                    log.info(t.getMessage());
                }
            });
        }

    }
}


