package cn.edu.seu.alumni_server.dao.entity;

import lombok.Data;

import javax.persistence.Column;
import javax.persistence.Id;
import java.io.Serializable;
import java.util.Date;

@Data
public class Account implements Serializable {
    @Column(name = "account_id")
    @Id
    private Long accountId;

    private String name;

    /**
     * 男 1 女 2
     */
    private Integer gender;

    private Long birthday;

    @Column(name = "self_desc")
    private String selfDesc;

    private String avatar;

    private String city;

    private String openid;

    private String phone;

    private String wechat;

    private String email;

    private String industry;

    /**
     * 0学生1工作
     */
    private Boolean type;

    /**
     * 0未注册1已注册
     */
    private Boolean registered;
    @Column(name = "step1_finished")
    private Boolean step1Finished;

    @Column(name = "c_time", insertable = false)
    private Date cTime;

    @Column(name = "u_time", insertable = false)
    private Date uTime;

    /**
     * 1有效0无效
     */
    @Column(name = "valid_status", insertable = false)
    private Boolean validStatus;

    private static final long serialVersionUID = 1L;

}