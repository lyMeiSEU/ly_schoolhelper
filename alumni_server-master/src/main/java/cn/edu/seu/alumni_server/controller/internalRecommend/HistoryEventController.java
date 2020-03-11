package cn.edu.seu.alumni_server.controller.internalRecommend;

import cn.edu.seu.alumni_server.common.Utils;
import cn.edu.seu.alumni_server.common.dto.WebResponse;
import cn.edu.seu.alumni_server.controller.dto.enums.EventType;
import cn.edu.seu.alumni_server.dao.entity.HistoryEvent;
import cn.edu.seu.alumni_server.dao.mapper.HistoryEventMapper;
import org.apache.commons.lang3.EnumUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import javax.servlet.http.HttpServletRequest;

@RestController
@RequestMapping("/event")
public class HistoryEventController {

    @Autowired
    HistoryEventMapper historyEventMapper;
    @Autowired
    HttpServletRequest request;

    @PostMapping("/")
    public WebResponse viewAPost(@RequestParam Long postId,
                                 @RequestParam String eventType) {
        Long accountId = (Long) request.getAttribute("accountId");

        if (!EnumUtils.isValidEnum(EventType.class, eventType)) {
            return new WebResponse().fail("eventType错误");
        }

        HistoryEvent historyEvent = new HistoryEvent();
        historyEvent.setAccountId(accountId);
        historyEvent.setPostId(postId);

        historyEvent.setEventId(Utils.generateId());
        historyEvent.setEventType(eventType);

        historyEventMapper.insertSelective(historyEvent);
        return new WebResponse();
    }

}
