package cn.edu.seu.alumni_server.dao.entity;

import lombok.Data;

import javax.persistence.Column;
import javax.persistence.Table;
import java.io.Serializable;
import java.util.Date;

@Table(name = "history_event")
@Data
public class HistoryEvent implements Serializable {
    @Column(name = "event_id")
    private Long eventId;

    @Column(name = "account_id")
    private Long accountId;

    private Date timestamp;

    @Column(name = "event_type")
    private String eventType;

    @Column(name = "post_id")
    private Long postId;

    private static final long serialVersionUID = 1L;
}