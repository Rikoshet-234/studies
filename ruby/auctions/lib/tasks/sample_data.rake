
namespace :db do
  desc 'Fill database with sample data.'

  task populate: :environment do
    create_users
    create_auctions
    create_tags
    create_bids
  end

end

def create_users(count = 20)
  puts 'Creating root user...'
  root_data = {
    name: 'Walter White',
    email: 'heisenberg@example.com',
    password: 'boskihania',
    password_confirmation: 'boskihania'
  }
  User.create!(root_data)

  bar = Progressbar.new(count)
  bar.start("Creating #{count} fake users...")
  count.times do |i|
    user_data = {
      name: Faker::Name.name,
      email: Faker::Internet.email,
      password: 'foobar',
      password_confirmation: 'foobar'
    }
    User.create!(user_data)
    bar.update(i + 1)
  end
  bar.finish
end

def create_auctions(count = 60)
  users = User.all

  bar = Progressbar.new(count)
  bar.start("Creating #{count} fake auctions...")
  count.times do
    user = users.sample
    auction_data = {
      title: Faker::Company.catch_phrase,
      content: Faker::Lorem.paragraph(20),
      finish_date: Time.now + [1, 2, 3, 4, 5, 6, 7].sample.days
    }
    user.auctions.create!(auction_data)
    bar.increase
  end
  bar.finish
end

def create_tags(count = 20)
  puts "Creating #{count} fake tags..."
  names = Faker::Lorem.words(count)

  auctions = Auction.all

  bar = Progressbar.new(auctions.count)
  bar.start('Assigning them with auctions...')
  auctions.each do |auction|
    tag_count = [1, 2, 3, 4, 5].sample
    auction.update_attributes(tag_list: names.sample(tag_count).join(', '))
    bar.increase
  end
  bar.finish
end

def create_bids(count = 4)
  tries = (1..count).to_a

  users = User.all
  bar = Progressbar.new(users.count)
  bar.start("Creating less than #{count} fake bids per auction...")
  Auction.all.each do |auction|
    # Starting bid
    basic = (rand * 100).to_i

    tries.sample.times do |i|
      basic = (basic + [1, 2, 3].sample) * 1.2
      bid = Bid.new(value: basic)
      bid.auction = auction
      bid.user = users.sample

      bid.save!
    end

    bar.increase
  end

  bar.finish
end

class Progressbar

  def initialize(max = 100, width = 40)
    @max = max
    @width = width
  end

  def start(message = '')
    @current = 0
    puts message
  end

  def increase(i = 1)
    update(@current + i)
  end

  def update(current)
    @current = current
    print "["
    (1..@width).each do |i|
      print (i <= @current.fdiv(@max) * @width ? '|' : ' ')
    end
    print "] "
    print "#{(current.fdiv(@max) * 100).round}%"

    print "\r"
  end

  def finish(message = 'Finished!')
    @current = @max
    puts message + ' ' * (@width - message.length + 3)
  end

end
