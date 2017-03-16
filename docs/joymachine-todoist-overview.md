# Todoist

-  **Page Details** 
	
	**Created By**
	trent

	**Created:** 
	Yesterday at 6:48 PM

	**Updated:** 
	Yesterday at 6:49 PM
---

## Overview

Todoist is our task management tool for [Joy Machine](http://joy-machine.com/) and [_Steel Hunters_](https://joy-machine.com/wp-content/steelhunters). There are an abundance of reasons that we've decided on this as opposed to traditional tasking solutions like JIRA, Asana, Trello, etc.

- Trent hates Kanban (Trello).
- Todoist is a very light, customizable solution for project management.
	- This allows us a great deal of flexibility in how we setup our particular task assignment/update/completion process.
	- While not initially intuitive, Todoist offers a lot of flexibility with filters and labels that make customized "dashboards" incredibly easy to define (once you know the query strings).
- Todoist has a very solid interpreter whenever you type out a task:
	- "Implement an MMO by Tuesday at 3:30pm." — This will translate to a task for "Implement an MMO" with a deadline of the following Tuesday at 3:30pm.
	- "Meet with Trent every Wednesday at 11:30pm" — This translates to a recurring task every Wednesday at 11:30 for "Meet with Trent".
	- "Game doesn't work. @bug p1 #steelhunters +trent" — This translates to a task "Game doesn't work.", filed with a label of "bug" (any number of labels can be specified), put into the project "steelhunters" (limit is one project), and assigns the task to Trent (limit is one assignee).
		- If you want to notify multiple people on any changes to the task or any comments, just create a comment after creating the task and tag people in the notification panel as-desired.
	- If you're not a fan of the string-based shortcuts, the GUI has components to accomplish all of the same functionality.
- Sharing projects with differing groups of people is absolutely trivial.
- Todoist is almost too good at emailing/pushing notifications based on any changes made.
- If there's a website in existence, it almost assuredly has a Todoist integration. 
- The Todoist web-app is incredibly solid, but they also have a native application for every platform.
	-  [**Link to Download Page**](https://todoist.com/overview) .
- Finally, Todoist has a really well-done REST API for querying details on tasks/projects/etc., which we can integrate into our website for communicating progress to our community.
- Trent really, really, really hates Kanban.

###  **Comment Threads** 

While we all use and read Slack and coordinate efforts and tasks and such using that, Slack is not a useful tool for status updates, questions, issues, schedule updates (including if you think you'll miss the assigned date — which isn't usually a problem, but it's helpful for us to know), and, most importantly, **when you are blocked on something**.

Whenever you comment, you can always choose to notify anyone you wish on the project just to ensure people are aware of your updates. At the minimum, ensure you mark Trent and Elgn regardless of what the task is about, as we are running the production/schedule for the project (Elgn moreso than Trent, though). You should also opt-in anyone that's related to the task as well.

 **Finally** : Even I forget this from time-to-time (always), but Todoist has complete support for file/image attachments in comment threads. So feel free to use and abuse that.

### Karma

There's a ["Karma" functionality built-in to Todoist](https://support.todoist.com/hc/en-us/articles/206209959-Tracking-productivity-with-Karma) which, essentially, is just a bit of gamification/statistics to try and encourage consistent task completion/monitoring. It's stupid and simple, but also it can appeal to people. Like Trent, who loves stats more than most people.

### Labels

Labels are, well, labels (or tags, more appropriately). They are just ways of classifying the miscellaneous areas of the project a given task is related to. It's helpful for creating search filters as well. For the most part, you can just choose a task's label based on the available ones we've setup, **but there is one very important label you must all use:** 

### The "Blocked" Label

Of all the varying labels, the "Blocked" label ( `@blocked` ) is an important way that Elgn, Melissa, and Trent can all quickly identify tasks that have been blocked (always leave a comment in the task when you mark a task as blocked — including tagging the relevant people for notification. 

 **The Rest of the Labels** 

-  `bug` 
-  `team` 
-  `business` 
-  `content` 
-  `engine` 
-  `gameplay` 
-  `backend` 
-  `networking` 
-  `design` 
-  `ui/ux` 
-  `docs` 
-  `tools` 
-  `meeting` 
-  `marketing` 
-  `site` 
-  `future-work` — This label is exempt from most search queries related to due dates or lack of assigned team member.
- And then there are always labels for relevant upcoming milestones.

### Filters

Filters are the absolute best thing ever. It sounds like an incredibly simple thing, but Todoist has a unique search system that allows you to setup and customize your filters for whatever things you need to track. Whether it's "Assigned to Me", "Overdue Tasks", etc.

The most important part of filters is that you can set your opening screen/page in Todoist to whatever filter is most useful for you. You can find this in "Todoist Settings" as "Start Page."

Unfortunately, as of the time of writing, we cannot simply share filters or create team-wide filters, so you'll need to create your own manually. LUCKILY WE HAVE A LIST BELOW!

---

###  **Helpful Filters and Their Query String** 

 **My Tasks** 

 `assigned to: me` 

 **View All** 

 `view all` 

 **My Tasks — Today** 

 `assigned to: me & today` 

 **My Tasks — Blocked** 

 `assigned to: me & @blocked` 

 **My Tasks — Overdue** 

 `assigned to: me & over due` 

 **My Tasks — No Due Date** 

 `assigned to: me & no date & !(@future-work)` 

 **Team's Tasks — Blocked** 

 `shared & @blocked` 

 **Team's Tasks — Today** 

 `shared & today` 

 **Team's Tasks — Overdue** 

 `shared & over due` 

 **Team's Tasks — Unassigned** 

 `shared & !assigned`
