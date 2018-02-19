# Contributing to MAVLink

We follow the [Github flow](https://guides.github.com/introduction/flow/) development model. Contributions fall into two main categories: Design and micro-service changes include new features that come with a state machine and message specifications for a new type of interface (examples: parameter or mission protocol). These are major contributions requiring a lot of vetting and should come with a RFC pull request in https://github.com/mavlink/rfcs. Protocol specification and documentation changes are usually changes with less impact and can be directly raised as pull requests against this repository.

How to raise a pull request is documented in the section below.

## How to contribute design and micro-service changes

  * Open a pull request against the RFC repository containing a new RFC number https://github.com/mavlink/rfcs and use the template in the 0000 RFC.
  * Reach out to the community on Slack and on http://discuss.dronecode.org to raise awareness
  * Address concerns by pushing more commits to the pull request

## How to contribute protocol specification changes

  * Open a pull request against the specifion repository: https://github.com/mavlink/mavlink
  * Reach out to the community on Slack and on http://discuss.dronecode.org to raise awareness
  * Address concerns by pushing more commits to the pull request
  
## How to open a pull request

### Fork the project, then clone your repo

First [fork and clone](https://help.github.com/articles/fork-a-repo) the project project.

### Create a feature branch

*Always* branch off master for new features.

```
git checkout -b mydescriptivebranchname
```

### Commit your changes

Always write descriptive commit messages and add a fixes or relates note to them with an [issue number](https://github.com/mavlink/mavlink) (Github will link these then conveniently)

**Example:**

```
Change the attitude output spec documentation

- Fixes a typo
- Clarifies that units are radians

Fixes issue #123
```

### Test your changes

Since we care about compatibility and safety, we will regularly ask you for test results.

### Push your changes

Push changes to your repo and send a [pull request](https://github.com/mavlink/mavlink/compare/).
