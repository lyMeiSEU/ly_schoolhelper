package cn.edu.seu.alumni_server.controller;

import cn.edu.seu.alumni_server.common.Utils;
import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.common.token.TokenUtil;
import cn.edu.seu.alumni_server.dao.entity.Account;
import cn.edu.seu.alumni_server.dao.entity.ConstMajor;
import cn.edu.seu.alumni_server.dao.entity.ConstSchool;
import cn.edu.seu.alumni_server.dao.mapper.AccountMapper;
import cn.edu.seu.alumni_server.dao.mapper.ConstMajorMapper;
import cn.edu.seu.alumni_server.dao.mapper.ConstSchoolMapper;
import cn.edu.seu.alumni_server.dao.mapper.V2ApiMapper;
import cn.edu.seu.alumni_server.service.QCloudFileManager;
import com.google.gson.Gson;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import javax.servlet.http.HttpServletRequest;
import java.io.IOException;
import java.util.List;
import java.util.Map;

/**
 * 系统测试用
 */
@RestController
@RequestMapping("/test")
@Slf4j
@SuppressWarnings("ALL")
public class TestController {

    @Autowired
    V2ApiMapper v2ApiMapper;
    @Autowired
    AccountMapper accountMapper;

    @Autowired
    ConstSchoolMapper constSchoolMapper;
    @Autowired
    ConstMajorMapper constMajorMapper;

    @Autowired
    QCloudFileManager qCloudFileManager;

    static {
    }

    @RequestMapping("/login")
    String login(@RequestParam(required = false) String accountId) {
        String token = TokenUtil.createToken(accountId);
        log.info("dev token: " + token);
        return token;
    }

    @RequestMapping("")
    String demo(@RequestParam(required = false) String a,
                @RequestBody(required = false) Map b) {
        System.out.println(a);
        System.out.println(b.toString());
        return "ok";
    }

    @Autowired
    HttpServletRequest request;

    @RequestMapping("/mysql")
    WebResponse test() {
        Long accountId = (Long) request.getAttribute("accountId");

        List<Account> list = v2ApiMapper.test(accountId);
        return new WebResponse().success(list);
    }

    @GetMapping("/account")
    WebResponse demo2(@RequestParam String openid) {
        Account accountNew = new Account();
        accountNew.setOpenid(openid);
        accountNew.setAccountId(Utils.generateId());
        accountMapper.insertSelective(accountNew);
        return new WebResponse().success(accountNew.getAccountId().toString());
    }

    @GetMapping("/school")
    WebResponse school(@RequestBody List<ConstSchool> constSchools) {
        constSchools.stream().forEach(constSchool -> {
            constSchoolMapper.insertSelective(constSchool);
        });
        return new WebResponse().success();
    }

    @GetMapping("/major")
    WebResponse major(@RequestBody List<ConstMajor> constMajors) {
        constMajors.stream().forEach(constMajor -> {
            constMajorMapper.insertSelective(constMajor);
        });
        return new WebResponse().success();
    }

    @PostMapping("/office/callback")
    String officeCallback(@RequestBody Map req) {

        System.out.println(new Gson().toJson(req));
        return "{\"error\":0}";
    }

    @PostMapping("/file")
    public String fileDemo(
            @RequestParam MultipartFile demo
    ) throws IOException {
        return qCloudFileManager.uploadOneFile(
                demo,
                String.valueOf(Utils.generateId()) + "." + demo.getOriginalFilename());
    }
}
