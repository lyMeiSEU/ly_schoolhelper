package cn.edu.seu.alumni_server.dao.entity;

import lombok.Data;

import javax.persistence.Column;
import javax.persistence.Id;
import java.io.Serializable;
import java.util.Date;

@Data
public class Message implements Serializable {
    @Column(name = "message_id")
    @Id
    private Long messageId;

    private Integer type;
    private String typeDesc;

    private String title;
    private String content;

    /**
     * 0未读1已读
     * 默认未读
     */
    private Integer status = 0;

    private Long timestamp;

    @Column(name = "c_time", insertable = false)
    private Date cTime;

    @Column(name = "u_time", insertable = false)
    private Date uTime;

    @Column(name = "valid_status", insertable = false)
    private Integer validStatus;

    @Column(name = "from_user")
    private Long fromUser;

    @Column(name = "to_user")
    private Long toUser;

    private static final long serialVersionUID = 1L;

}