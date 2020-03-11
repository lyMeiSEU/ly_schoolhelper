package cn.edu.seu.alumni_server.dataSync.entity;

import lombok.Data;

import javax.persistence.Table;
import java.io.Serializable;

@Table(name = "Personal")
@Data
public class Personal implements Serializable {
    private String openid;

    private String phone;

    private String wechat;

    private String email;

    private static final long serialVersionUID = 1L;
}