package cn.edu.seu.alumni_server.controller.dto;

import cn.edu.seu.alumni_server.dao.entity.Account;
import lombok.Data;
import org.springframework.beans.BeanUtils;

@Data
public class AccountDTO {
    private Long accountId;
    private String name;
    private Integer gender;
    private Long birthday;
    private String selfDesc;
    private String avatar;
    private String city;
    private String openid;
    private String phone;
    private String wechat;
    private String email;
    private String industry;
    private Boolean type;

    public AccountDTO() {
    }

    public AccountDTO(Account account) {
        BeanUtils.copyProperties(account, this);
        this.setBirthday(account.getBirthday());
    }

    public Account toAccount() {
        Account account = new Account();
        BeanUtils.copyProperties(this, account);
        account.setBirthday(this.getBirthday());
        return account;
    }
}
